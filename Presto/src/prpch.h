#pragma once

#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "Presto/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif