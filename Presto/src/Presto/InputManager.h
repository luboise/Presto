#pragma once

#include "Core.h"
#include <windows.h>
#include <xinput.h>

#include <atomic>
#include <thread>

#define MAX_XINPUT_PORTS 4
#define POLLING_RATE 2

namespace Presto {

struct Thumbstick {
    short X;
    short Y;
};

class PRESTO_API InputManager {
   public:
    static void Init();
    static void SetPort(DWORD port);
    static bool GetState();
    static void TogglePolling();

    static Presto::Thumbstick GetLeftThumbStickXY();
    


   private:
    static DWORD controller_port;
    static XINPUT_STATE controller_state;
    static void PollInputs(std::atomic<bool>& continue_polling);

    static std::atomic<bool> is_polling;
    static std::thread input_thread;
};



}  // namespace Presto