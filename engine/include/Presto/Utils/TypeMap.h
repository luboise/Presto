#pragma once

#include <any>
#include <unordered_map>

#include "Presto/Objects/Component.h"

namespace Presto {

class TypeMap {
   public:
    TypeMap() = default;

    template <typename T>
    static constexpr bool valid_type_v = requires { typename std::vector<T>; };

    template <typename Sub>
        requires valid_type_v<Sub>
    void add(Sub s) {
        std::vector<Sub>& vec = get<Sub>();
        vec.emplace_back.emplace_back(s);
    }

    template <typename Sub>
        requires valid_type_v<Sub>
    std::vector<Sub>& get() {
        ensureSubtype<Sub>();

        std::any& any = erasedMap_.at(ClassID<Sub>);
        std::vector<Sub>& vec = std::any_cast<std::vector<Sub>&>(any);

        return vec;
    }

   private:
    std::unordered_map<class_id_t, std::any> erasedMap_;

    template <typename Sub>
    void ensureSubtype() {
        class_id_t c_id{ClassID<Sub>};
        if (!erasedMap_.contains(c_id)) {
            erasedMap_[c_id] = std::make_any<std::vector<Sub>>();
        }
    };
};
}  // namespace Presto
