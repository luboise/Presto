#pragma once

#include "presto/core/assert.h"
#include "presto/core/constants.h"

namespace Pr {

/**
 * A class for generating and tracking IDs. This is to ensure there is no ID
 * clash, but also that IDs are recoverable after being unreserved.
 *
 * @tparam T The tyep of the id being allocated. This must be an unsigned
 * integral type, and typically will be a uint16_t or uint32_t
 */
template <typename T>
    requires std::unsigned_integral<T> && std::is_trivially_copyable_v<T>
class IDGenerator {
    static constexpr T INVALID_ID{0};

   public:
    /**
     * @brief Constructor for IDGenerator.
     * @param threshold  The threshold for reserved IDs. IDs under this
     * threshold must be reserved using @fn reserve.
     */
    explicit IDGenerator(T threshold = 1) {
        PR_CORE_ASSERT(threshold > 0,
                       "IDGenerator can't be instantiated with a value of 0, "
                       "as this is reserved for INVALID_ID");
        minGeneratedThreshold_ = threshold;
        currentId_ = minGeneratedThreshold_;
    }

    ~IDGenerator() = default;

    /**
     * @brief	Reserves an unused ID and returns it. This should generally be
     * used rather than reserve. If available, this function will prefer keys
     * that have just been released rather than new ones.
     * @return	The newly reserved ID.
     */
    T generate() {
        if (!releasedIds_.empty()) {
            auto key{std::move(releasedIds_.front())};
            releasedIds_.pop();

            return key;
        }

        return currentId_++;
    };

    /**
     *@brief	Attempts to reserve a specific below the minimum generated
     * threshold.
     * @param	id	The id which will be reserved.
     * @return	The newly generated id.
     */
    bool reserve(T id) {
        auto [it, did_insert] = ids_.insert(id);
        return did_insert;
    };

    /**
     * @brief	Returns a truthy value based on whether a given id is reserved
     * or not.
     */
    [[nodiscard]] bool reserved(T id) const noexcept {
        return ids_.contains(id);
    }

    /**
     * @brief	 Unreserves an ID so that it can be reused.
     */
    bool release(T id) {
        bool didErase = ids_.erase(id);

        if (didErase && id >= minGeneratedThreshold_) {
            releasedIds_.push(id);
        }

        return didErase;
    }

    IDGenerator(const IDGenerator&) = delete;
    IDGenerator(IDGenerator&&) = delete;
    IDGenerator& operator=(const IDGenerator&) = delete;
    IDGenerator& operator=(IDGenerator&&) = delete;

   private:
    T minGeneratedThreshold_{1};

    T currentId_{1};

    std::unordered_set<T> ids_;
    std::queue<T> releasedIds_;

    // TODO: Implement freeing IDs, as well as a queue of freedIds_
    // std::queue<T> freedIds_;
};

using allocator_id_t = PR_NUMERIC_ID;

template <typename K, typename V,
          bool HasDefault = std::is_default_constructible_v<V>>
    requires std::is_constructible_v<std::map<K, V>> &&
             std::is_constructible_v<IDGenerator<K>> && (!std::is_same_v<K, V>)
class Allocator {
    using return_t = std::pair<K, V*>;
    static consteval return_t BAD_INSERTION() { return {0, nullptr}; }

   public:
    explicit Allocator(K threshold = 1) : idGenerator_(threshold) {};

    [[nodiscard]] Pr::size_t size() const { return entries_.size(); }
    [[nodiscard]] bool empty() const { return entries_.size() == 0; }

    /**
     * @brief  Search for a value by key
     */
    [[nodiscard]] V* find(K key) {
        if (auto found{entries_.find(key)}; found != entries_.end()) {
            return (*found).second.get();
        }

        return nullptr;
    }

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
                       std::format("Keys that an Allocator receives from an "
                                   "IDGenerator must be unique, and {} wasn't.",
                                   new_id));

        return put(new_id, std::move(ptr));
    }

    return_t alloc(Allocated<V> allocation, K key = 0) {
        if (allocation == nullptr) {
            PR_CORE_ERROR(
                "An allocator can't allocate from a null pointer. Skipping "
                "this allocation.");
            return BAD_INSERTION();
        }

        if (std::ranges::any_of(entries_ | std::views::values,
                                [&allocation](const Allocated<V>& val) -> bool {
                                    return val == allocation;
                                })) {
            PR_CORE_ERROR(
                "An allocator can't allocate the same memory twice. This is a "
                "serious error.");
            return BAD_INSERTION();
        };

        K new_id{};

        if (key == 0) {
            new_id = idGenerator_.generate();
        } else {
            if (!idGenerator_.reserve(key)) {
                return BAD_INSERTION();
            }

            new_id = key;
        }

        PR_CORE_ASSERT(!entries_.contains(new_id),
                       std::format("Keys that an Allocator receives from an "
                                   "IDGenerator must be unique, and {} wasn't.",
                                   new_id));

        return put(new_id, std::move(allocation));
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

        Pr::size_t removed{entries_.erase(id)};
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

    // Iterators
    auto begin() { return entries_.begin(); }
    auto end() { return entries_.end(); }
    auto begin() const { return entries_.begin(); }
    auto end() const { return entries_.end(); }

   private:
    // const bool _useNestedID;

    std::map<K, Allocated<V>> entries_;
    IDGenerator<K> idGenerator_;

    [[nodiscard]] return_t put(K key, Allocated<V> val) {
        auto [it, emplaced]{entries_.emplace(key, std::move(val))};
        if (!emplaced) {
            return BAD_INSERTION();
        }

        return {it->first, it->second.get()};
    }
};
}  // namespace Pr
