#include "InputManager.h"
#include "Log.h"

namespace Presto {
    DWORD InputManager::controller_port;
    XINPUT_STATE InputManager::controller_state;
    std::atomic<bool> InputManager::is_polling;

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

    Thumbstick InputManager::GetLeftThumbStickXY() {
        GetState();

        return Thumbstick(controller_state.Gamepad.sThumbLX,
                            controller_state.Gamepad.sThumbLY);
    }

    void InputManager::PollInputs(std::atomic<bool>& continue_polling) {
        using namespace std::chrono_literals;

        
        const auto spacing = (1000ms / POLLING_RATE).count();

        auto prevTime = std::chrono::steady_clock::now();

        std::chrono::steady_clock::time_point currentTime;
        long long duration;

        while (continue_polling) {
            GetState();
            currentTime = std::chrono::steady_clock::now();

            // Duration of previous poll
            duration = std::chrono::duration_cast<std::chrono::microseconds>(
                           currentTime - prevTime)
                           .count();

            PR_INFO("Length of previous iteration: {} ~= {}ms",
                duration,
                (currentTime - prevTime) / 1ms
            );  // using milliseconds and seconds accordingly

            size_t step_count = 0;
            while (duration > 0) {
                step_count++;
                duration -= spacing;
            }

            while (duration > spacing) {
                duration -= spacing;
                prevTime +=
                    std::chrono::duration_cast<std::chrono::microseconds>(
                        spacing);
                spacing;
                std::this_thread::sleep_for(spacing - duration);
                prevTime += spacing;
            } else {
                // Need to fix this to round up to the nearest spacing
                prevTime += duration;
            }

            PR_INFO("Waiting for: {}ms", wait_time);  // using milliseconds and seconds accordingly
        };
    }

 }  // namespace Presto