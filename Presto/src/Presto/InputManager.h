#pragma once

#include "Core.h"
#include "Xinput.h"

#define MAX_XINPUT_PORTS 4
#define POLLING_RATE 1

namespace Presto {

class PRESTO_API InputManager {
   public:
    static void Init();
    static void SetPort(DWORD port);
    static bool GetState();
    static void TogglePolling();

    static Thumbstick GetLeftThumbStickXY();
    


   private:
    static DWORD controller_port;
    static XINPUT_STATE controller_state;
};

struct Thumbstick {
    short X;
    short Y;
};

}  // namespace Presto