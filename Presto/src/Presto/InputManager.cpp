#include "InputManager.h"
#include "Log.h"


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
            PR_CORE_ERROR("Controller in port {} is not connected.", controller_port);
        }
            
	}

    void InputManager::SetPort(DWORD port) {
        if (port >= MAX_XINPUT_PORTS) {
            PR_CORE_ERROR("Invalid port given. Continuing to use port {}", controller_port);
        } else {
            controller_port = port;
        }
    }

    bool InputManager::GetState() {
        bool read_success =
            XInputGetState(controller_port, &controller_state);

        return (read_success == ERROR_SUCCESS);
    }

    void InputManager::TogglePolling() {
        // If not polling, start a new thread
        if (!is_polling) {
            is_polling = true;

            // std::ref used as thread makes a full copy of the variable, and we need a reference
            input_thread = std::thread(PollInputs, std::ref(is_polling));
        }
        // Otherwise, end the polling thread
        else {
            is_polling = false;
            input_thread.join();
        }
    }

    void InputManager::PollInputs(std::atomic<bool>& continue_polling) {        
        const auto spacing = (1000ms / POLLING_RATE);

        auto prevTime = std::chrono::steady_clock::now();

        std::chrono::steady_clock::time_point currentTime;
        std::chrono::milliseconds duration;

        while (continue_polling) {
            GetState();
            LogGamepad();
            currentTime = std::chrono::steady_clock::now();

            // Duration of previous poll
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                           currentTime - prevTime);

            //PR_INFO("Length of previous iteration: {} ~= {}ms",
            //    duration.count(),
            //    ((currentTime - prevTime) / 1ms)
            //);  // using milliseconds and seconds accordingly

            size_t step_count = 0;
            while (currentTime > prevTime) {
                step_count++;
                prevTime += spacing;
            }

            assert(step_count > 0);

            if (step_count > 1) {
                PR_INFO("Skipped {} polls.", step_count - 1);
            }

            std::this_thread::sleep_until(prevTime);
        };
    }

    void InputManager::LogGamepad() {
        PR_INFO("X: {}   Y: {}",
                InputManager::controller_state.Gamepad.sThumbLX,
                InputManager::controller_state.Gamepad.sThumbLY);
    }

 }  // namespace Presto