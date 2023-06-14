#include <iostream>
#include <Windows.h>
#include "WindowsInputPoller.h"
#include <assert.h>

typedef unsigned __int64 QWORD;

static bool running = true;
UINT MSG_GETRIBUFFER = RegisterWindowMessage("MSG_GETRIBUFFER");

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

    ShowWindow(hwnd, SW_SHOW);

    while (running)
    {
        PostMessage(hwnd, MSG_GETRIBUFFER, 0, 0);
        // std::cout << "Value of a: " << (isDown ? "o" : "x") << std::endl;
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

    case MSG_GETRIBUFFER:
        doPoll();
        break;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void doPoll()
{
    UINT cbSize;
    Sleep(1000);

    if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)) != 0)
    {
        throw std::invalid_argument("NULL input buffer has a size greater than 0. Invalid access.");
    }

    cbSize *= 16; // up to 16 messages
    std::cout << "Allocating " << cbSize << " bytes" << std::endl;
    PRAWINPUT pRawInput = (PRAWINPUT)malloc(cbSize);
    if (pRawInput == NULL)
    {
        std::cout << "Unable to allocate memory for buffer read." << std::endl;
    }

    for (;;)
    {
        UINT cbSizeT = cbSize;
        UINT nInput = GetRawInputBuffer(pRawInput, &cbSizeT, sizeof(RAWINPUTHEADER));
        std::cout << "nInput = " << nInput << std::endl;
        if (nInput == 0)
        {
            break;
        }

        assert(nInput > 0);
        PRAWINPUT *paRawInput = (PRAWINPUT *)malloc(sizeof(PRAWINPUT) * nInput);
        if (paRawInput == NULL)
        {
            std::cout << "paRawInput NULL" << std::endl;
            break;
        }

        PRAWINPUT pri = pRawInput;
        for (UINT i = 0; i < nInput; ++i)
        {
            std::cout << " input[" << i << "] = @" << pri << std::endl;
            paRawInput[i] = pri;
            pri = NEXTRAWINPUTBLOCK(pri);
        }

        free(paRawInput);
    }
    free(pRawInput);
}