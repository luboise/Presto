#pragma once

#include <cstdint>
#include <type_traits>
#include <unordered_map>

#include "Presto/Core/Concepts.h"
#include "Presto/Objects/Component.h"

template <typename T>
concept valid_subtype = std::is_trivially_copyable_v<T>;

namespace Presto {
template <class T>
    requires std::is_trivially_copyable_v<T>
class SubTypeMap {
   public:
    SubTypeMap() = default;

    template <typename Sub>
        requires DerivedFrom<Sub, T, STRICT> && valid_subtype<Sub>
    void add(Sub s) {
        ensureSubtype<Sub>();
        subtypeMap_[ClassID<Sub>].emplace_back(std::move(s));
    }

   private:
    std::unordered_map<std::uint32_t, std::vector<T>> subtypeMap_;

    template <typename Sub>
    void ensureSubtype() {
        class_id_t c_id{ClassID<Sub>};
        if (!subtypeMap_.contains(c_id)) {
            subtypeMap_[c_id] = std::vector<T>{};
        }
    };
};
}  // namespace Presto
