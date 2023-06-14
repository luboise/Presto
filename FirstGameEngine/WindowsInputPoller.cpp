#include "WindowsInputPoller.h"
#include <stdexcept>
#include <iostream>

WindowsInputPoller::WindowsInputPoller(HINSTANCE hInstance) {
    this->Rid[0].usUsagePage = 0x01;
    this->Rid[0].usUsage = 0x06;
    this->Rid[0].dwFlags = RIDEV_NOLEGACY;
    this->Rid[0].hwndTarget = 0;

    if (RegisterRawInputDevices(this->Rid, 1, sizeof(this->Rid[0])) == FALSE)
    {
        //registration failed. Call GetLastError for the cause of the error
        throw std::exception("Invalid assignment.");
    };

}

bool WindowsInputPoller::poll() {
    UINT cbSize;
    Sleep(1000);

    if (GetRawInputBuffer(NULL, &cbSize, sizeof(RAWINPUTHEADER)) != 0) {
        throw std::invalid_argument("NULL input buffer has a size greater than 0. Invalid access.");
    }

    cbSize *= 16; // up to 16 messages
    // Log(_T("Allocating %d bytes"), cbSize);
    PRAWINPUT pRawInput = (PRAWINPUT)malloc(cbSize);
    if (pRawInput == NULL)
    {
        throw std::runtime_error("Unable to allocate memory for buffer read.");
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

        ASSERT(nInput > 0);
        PRAWINPUT* paRawInput = (PRAWINPUT*)malloc(sizeof(PRAWINPUT) * nInput);
        if (paRawInput == NULL)
        {
            Log(_T("paRawInput NULL"));
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
};