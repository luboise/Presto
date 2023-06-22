#pragma once

#include "Core.h"
#include <windows.h>
#include <xinput.h>

#define MAX_XINPUT_PORTS 4
#define POLLING_RATE 1

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
};



}  // namespace Presto