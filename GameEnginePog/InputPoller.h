#pragma once

#include <vector>
#include <windows.h>

using std::vector;

class InputPoller {
public:
	InputPoller();
	void poll();
private:
	RAWINPUTDEVICE Rid[1];
};