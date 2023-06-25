#include "InputManager.h"

#include "Presto/Log.h"
#include "prpch.h"

using namespace std::chrono_literals;

namespace Presto {
    DWORD InputManager::controller_port;
    XINPUT_STATE InputManager::controller_state;
    std::atomic<bool> InputManager::is_polling;
    std::thread InputManager::input_thread;

    void InputManager::Init() {
        // Set port to 0 (controller 1)
        SetPort(0);

        is_polling = false;

        // Clear state (initialise to 0)
        ZeroMemory(&controller_state, sizeof(XINPUT_STATE));

        // Ensure controller is connected
        if (!GetState()) {
            PR_CORE_ERROR("Controller in port {} is not connected.",
                          controller_port);
        }
    }

    void InputManager::SetPort(DWORD port) {
        if (port >= MAX_XINPUT_PORTS) {
            PR_CORE_ERROR("Invalid port given. Continuing to use port {}",
                          controller_port);
        } else {
            controller_port = port;
        }
    }

    bool InputManager::GetState() {
        bool read_success = XInputGetState(controller_port, &controller_state);

        return (read_success == ERROR_SUCCESS);
    }

    void InputManager::TogglePolling() {
        // If not polling, start a new thread
        if (!is_polling) {
            is_polling = true;

            // std::ref used as thread makes a full copy of the variable, and we
            // need a reference
            input_thread = std::thread(PollInputs, std::ref(is_polling));
        }
        // Otherwise, end the polling thread
        else {
            is_polling = false;
            input_thread.join();
        }
    }

    void InputManager::PollInputs(std::atomic<bool>& continue_polling) {
        const auto spacing =
            std::chrono::duration_cast<std::chrono::nanoseconds>(1s /
                                                                 POLLING_RATE);

        auto prevTime = std::chrono::steady_clock::now();

        auto log_lambda = [](std::atomic<bool>& is_polling, auto wait_length) {
            while (is_polling) {
                LogGamepad();
                std::this_thread::sleep_for(wait_length);
            };
        };

        // Initialise logger_thread
        std::thread logger_thread(log_lambda, std::ref(continue_polling),
                                  spacing);

        std::chrono::steady_clock::time_point currentTime;
        std::chrono::nanoseconds duration;

        while (continue_polling) {
            std::this_thread::sleep_until(prevTime);
            GetState();
            currentTime = std::chrono::steady_clock::now();

            // Duration of previous poll
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
                currentTime - prevTime);

            // PR_INFO("Length of previous iteration: {} ~= {}ms",
            //     duration.count(),
            //     ((currentTime - prevTime) / 1ms)
            //);  // using milliseconds and seconds accordingly

            size_t step_count = 0;
            while (currentTime > prevTime) {
                step_count++;
                prevTime += spacing;
            }

            assert(step_count > 0);

            if (step_count > 1) {
                PR_WARN("Skipped {} polls.", step_count - 1);
            }
        };

        // Ensure the logger thread terminates when this thread is terminating
        logger_thread.join();
    }

    void InputManager::LogGamepad() {
        PR_INFO("X: {}   Y: {}",
                InputManager::controller_state.Gamepad.sThumbLX,
                InputManager::controller_state.Gamepad.sThumbLY);
    }

}  // namespace Presto