#include <iostream>
#include <Windows.h>
#include "WindowsInputPoller.h"

static bool running = true;

// Callback function occurs whenever windows does
// literally anything concerning the window
// eg resizing the window, minimising it, maximising it etc
LRESULT CALLBACK platform_windows_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    LPCSTR MAIN_WC_NAME = "Sample Window Class";

    WNDCLASS MAIN_WC = {};
    MAIN_WC.lpfnWndProc = platform_windows_callback;
    MAIN_WC.hInstance = hInstance;
    MAIN_WC.lpszClassName = MAIN_WC_NAME;

    // Register the main window class
    if (!RegisterClass(&MAIN_WC))
    {
        MessageBoxA(0, "Failed to register main window class.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles.
        MAIN_WC_NAME,        // Window class
        "The funny pog",     // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    // Return -1 if window does not initialise, failed to start
    if (hwnd == NULL)
    {
        return -1;
    }

    WindowsInputPoller poller(hInstance);
    while (true)
    {
        bool isDown = poller.poll();
        std::cout << "Value of a: " << (isDown ? "o" : "x") << std::endl;
    }

    return 0;
}

LRESULT CALLBACK platform_windows_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        running = false;
        break;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}