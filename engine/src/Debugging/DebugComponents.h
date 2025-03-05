#pragma once

#include <type_traits>
#include "imgui.h"

namespace Presto {

template <typename T>
struct EnumMember {
    T value;
    const char* label;
};

namespace DebugComponents {

template <typename EnumType>
    requires std::is_enum_v<EnumType>
void EnumChooser(EnumType& val, std::vector<EnumMember<EnumType>> members) {
    bool show = false;

    for (const auto& member : members) {
        show = val == member.value;

        if (ImGui::Checkbox(member.label, &show)) {
            val = member.value;
        }
    }
};

template <typename T, typename Arg, typename CallbackT = std::function<void(T)>>
    requires std::is_integral_v<T> && std::is_convertible_v<Arg, T>
void SliderChooser(T& val, const char* label, Arg min, Arg max,
                   CallbackT callback = nullptr) {
    int temp{static_cast<int>(val)};

    if (ImGui::SliderInt(label, &temp, min, max)) {
        if constexpr (std::is_invocable_v<CallbackT, T>) {
            callback(static_cast<T>(temp));
        } else {
            val = static_cast<T>(temp);
        }
    };
}

template <typename T, typename CallbackT = std::function<void(T)>>
    requires std::is_floating_point_v<T>
void SliderChooser(T& val, const char* label, T min, T max,
                   CallbackT callback) {
    T temp{val};

    if (ImGui::SliderFloat(label, &temp, min, max)) {
        if constexpr (std::is_invocable_v<CallbackT, T>) {
            callback(temp);
        } else {
            val = temp;
        }
    };
};

}  // namespace DebugComponents

}  // namespace Presto
