#include <iostream>
#include <Windows.h>
#include "WindowsInputPoller.h"
#include <assert.h>

#include <chrono>
#include <thread>

#define SECONDS_TO_RUN 10
#define TICKS_PER_SECOND 250

#define MSG_GETRIBUFFER 0x0400

using namespace std::chrono_literals;

typedef unsigned __int64 QWORD;

static bool running = true;


// Callback function occurs whenever windows does
// literally anything concerning the window
// eg resizing the window, minimising it, maximising it etc
LRESULT CALLBACK platform_windows_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND setupWindow(HINSTANCE hInstance);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HWND hwnd = setupWindow(hInstance);

    // Return -1 if window does not initialise, failed to start
    if (hwnd == NULL)
    {
        return -1;
    }

    WindowsInputPoller poller(hInstance);

    ShowWindow(hwnd, SW_SHOW);

    const auto spacing = 1000ms / TICKS_PER_SECOND;

    auto prevTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point currentTime;
    std::chrono::microseconds duration;

    auto TOTAL_TICKS = TICKS_PER_SECOND * SECONDS_TO_RUN;

    for (auto i = 0; i < TOTAL_TICKS; i++)
    {
        currentTime = std::chrono::steady_clock::now();
        
        duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - prevTime);
        std::cout
            << "Length of previous iteration"
            << duration << " ≈ " << (currentTime - prevTime) / 1ms << "ms" << std::endl;  // using milliseconds and seconds accordingly

        if (duration < spacing) {
            std::this_thread::sleep_for(spacing - duration);
            prevTime += spacing;
        } else {
            prevTime += duration;
        }

        std::cout << "Sending new message." << std::endl;
        SendMessage(hwnd, MSG_GETRIBUFFER, 0, 0);
        
    };

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
        // doPoll();
        std::cout << "MESSAGE RECEIVED!" << std::endl;
        break;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

HWND setupWindow(HINSTANCE hInstance)
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
        return NULL;
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

    return hwnd;
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