#include "presto/core/types.h"

namespace Presto {
namespace Input {
using KeyCodeType = Presto::uint32_t;

enum class Key : KeyCodeType {
    INVALID_KEY = 0x0000,
    LEFT_ARROW = 0x0001,
    DOWN_ARROW = 0x0002,
    UP_ARROW = 0x0003,
    RIGHT_ARROW = 0x0004,
    SPACEBAR = 0x0005,
};

constexpr KeyCodeType CodeOf(Key key) noexcept {
    return static_cast<KeyCodeType>(key);
}

}  // namespace Input
using namespace Input;

}  // namespace Presto
