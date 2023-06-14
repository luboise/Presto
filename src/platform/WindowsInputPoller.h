#pragma once

#include <Windows.h>

class WindowsInputPoller
{
public:
    WindowsInputPoller(HINSTANCE hInstance);
    bool poll();

private:
    RAWINPUTDEVICE Rid[1];
};