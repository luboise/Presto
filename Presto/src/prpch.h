#pragma once

#include <iostream>
#include <memory>
#include <atomic>
#include <thread>

#include <string>
#include <sstream>
#include <vector>

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <Xinput.h>
#endif