#include "InputManager.h"
#include "Log.h"

namespace Presto {
    DWORD InputManager::controller_port;
    XINPUT_STATE InputManager::controller_state;

	void InputManager::Init() {
        // Set port to 0 (controller 1)
		SetPort(0);

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

 }  // namespace Presto