#pragma once

#include "Presto/Core/Constants.h"
#include "Utils/IDGenerator.h"

#include <algorithm>
#include <concepts>
#include <map>
#include <type_traits>

namespace Presto {
using allocator_id_t = PR_NUMERIC_ID;

template <typename K, typename V,
          bool HasDefault = std::is_default_constructible_v<V>>
    requires std::is_constructible_v<std::map<K, V>> &&
             std::is_constructible_v<IDGenerator<K>> && (!std::is_same_v<K, V>)
class Allocator {
    using return_t = std::pair<K, V*>;
    static consteval return_t BAD_INSERTION() { return {0, nullptr}; }

   public:
    Allocator() = default;

    return_t alloc()
        requires std::default_initializable<V>
    {
        alloc(V());
    }

    return_t alloc(V* allocation) {
        if (allocation == nullptr) {
            PR_CORE_ERROR(
                "An allocator can't allocate from a null pointer. Skipping "
                "this allocation.");
            return BAD_INSERTION();
        }

        if (std::ranges::any_of(entries_ | std::views::values,
                                [allocation](const Allocated<V>& val) -> bool {
                                    return val == allocation;
                                })) {
            PR_CORE_ERROR(
                "An allocator can't allocate the same memory twice. This is a "
                "serious error.");
            return BAD_INSERTION();
        };

        Allocated<V> ptr{allocation};

        V new_id{idGenerator_.generate()};
        PR_CORE_ASSERT(!entries_.contains(new_id),
                       "Keys that an Allocator receives from an IDGenerator "
                       "must be unique, and {} wasn't.",
                       new_id);

        put(new_id, std::move(ptr));
    }

    return_t alloc(Allocated<V> allocation) {
        if (allocation == nullptr) {
            PR_CORE_ERROR(
                "An allocator can't allocate from a null pointer. Skipping "
                "this allocation.");
            return BAD_INSERTION();
        }

        if (std::ranges::any_of(entries_ | std::views::values,
                                [allocation](const Allocated<V>& val) -> bool {
                                    return val == allocation;
                                })) {
            PR_CORE_ERROR(
                "An allocator can't allocate the same memory twice. This is a "
                "serious error.");
            return BAD_INSERTION();
        };

        V new_id{idGenerator_.generate()};
        PR_CORE_ASSERT(!entries_.contains(new_id),
                       "Keys that an Allocator receives from an IDGenerator "
                       "must be unique, and {} wasn't.",
                       new_id);

        put(new_id, std::move(allocation));
    }

    return_t alloc(V&& new_value)
        requires std::move_constructible<V>
    {
        V new_id{idGenerator_.generate()};

        Allocated<V> ptr{std::make_unique(std::move(new_value))};
        entries_.emplace(new_id, std::move(ptr));
    }

    bool release(K id) {
        if (!idGenerator_.release(id)) {
            PR_WARN(
                "Unable to release id {} in Allocator. Ignoring this release.",
                id);
            return false;
        }

        PR_CORE_ASSERT(entries_.contains(id),
                       "Attempted to release memory that has already been "
                       "released. ID: {}",
                       id);

        std::size_t removed{entries_.erase(id)};
        PR_CORE_ASSERT(
            removed > 0,
            "Allocator release should never fail after checking the ID.");

        return true;
    }

    /**
     * @brief   Finds an allocated value V in the list of entries and releases
     * it. This frees its ID.
     */
    void release(V query)
        requires std::equality_comparable<V>
    {
        PR_CORE_ASSERT(query != nullptr,
                       "Attempted to release memory from a nullptr.");

        auto values_view{entries_ | std::views::values};

        auto found{std::ranges::find_if(
            values_view,
            [query](const Allocated<V>& val) { return *val == query; })};

        if (!found) {
            return;
        }

        this->release(*found.first);
    }

   private:
    // const bool _useNestedID;

    std::map<K, V> entries_;
    IDGenerator<K> idGenerator_;

    return_t put(K key, Allocated<V> val) {
        auto [it, emplaced]{entries_.emplace(key, std::move(val))};
        if (!emplaced) {
            return BAD_INSERTION();
        }

        return *emplaced;
    }
};

}  // namespace Presto
