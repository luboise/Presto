#pragma once

#include <Xinput.h>

#include "Presto/Core.h"

#define MAX_XINPUT_PORTS 4
#define POLLING_RATE 250

namespace Presto {
    class PRESTO_API InputManager {
       public:
        static void Init();
        static void SetPort(DWORD port);
        static bool GetState();
        static void TogglePolling();

       private:
        static DWORD controller_port;
        static XINPUT_STATE controller_state;
        static std::atomic<bool> is_polling;
        static std::thread input_thread;

        static void PollInputs(std::atomic<bool>& continue_polling);
        static void LogGamepad();
    };

}  // namespace Presto