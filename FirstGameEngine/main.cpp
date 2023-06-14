#include <iostream>
#include <Windows.h>
#include "WindowsInputPoller.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t MAIN_WC_NAME[] = L"Sample Window Class";

    WNDCLASS MAIN_WC = { };
    MAIN_WC.lpfnWndProc = WindowProc;
    MAIN_WC.hInstance = hInstance;
    MAIN_WC.lpszClassName = MAIN_WC_NAME;

    // Register the main window class
    RegisterClass(&MAIN_WC);

    std::cout << "bullshit" << std::endl;

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        MAIN_WC_NAME,                   // Window class
        L"The funny pog",               // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    // Check if failed to initialise
    if (hwnd == NULL)
    {
        return 1;
    }

    WindowsInputPoller poller(hInstance);
    while (true) {
        bool isDown = poller.poll();
        std::cout << "Value of a: " << (isDown ? "o" : "x") << std::endl;
    }

	return 0;
}
