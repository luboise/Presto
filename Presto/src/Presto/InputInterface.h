#pragma once

#include "Core.h"
#include "InputManager.h"

#include <tuple>

namespace Presto {

class PRESTO_API InputInterface {
public:
	static bool IsButtonDown(GAMEPAD_BUTTON btn);
	static unsigned char GetDpadDirection();
    static std::pair<float, float> GetStickNormalised(THUMBSTICK stick);

private:
    static Presto::InputManager im;
};

enum class GAMEPAD_BUTTON {
	BTN_TOP,
	BTN_RIGHT,
	BTN_DOWN,
	BTN_LEFT,

	BTN_SHOULDER_LEFT,
	BTN_SHOULDER_RIGHT,

	BTN_SELECT,
	BTN_START,
};

enum class THUMBSTICK {
	LEFT,
	RIGHT
};

}  // namespace Presto