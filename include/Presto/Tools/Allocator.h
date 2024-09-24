#pragma once

#include <map>

namespace Presto {
    using id_t = uint32_t;

    template <typename T> /*, typename... AcquireArgs> >*/
    class Allocator {
        // typedef std::function<T(AcquireArgs...)> InstantiationFunction;
        using ValueType = T*;

       public:
        Allocator(/**bool nestedID = false**/)
            : _currentId(0) /**, _useNestedID(nestedID)**/ {}

        /**
T* Acquire(AcquireArgs... args) {
    auto new_t =
        _useNestedID ? new T(_currentId, args...) : new T(args...);

    this->_entries[_currentId++] = new_t;

    return new_t;
}
        **/

        void add(ValueType new_value) {
            PR_CORE_ASSERT(!isAllocated(new_value),
                           "Attempted to add a value that already exists in "
                           "the allocator: {}",
                           (fmt::ptr(new_value)))

            _entries[_currentId++] = new_value;
        }

        void release(id_t id) {
            PR_CORE_ASSERT(_entries.contains(id),
                           "Attempted to release memory that has already been "
                           "released. ID: {}",
                           id);

            PR_CORE_ASSERT(_entries[id] != nullptr,
                           "Attempted to release memory from a nullptr.")

            delete _entries[id];
            _entries.erase(id);
        }

        void release(ValueType value) {
            PR_CORE_ASSERT(value != nullptr,
                           "Attempted to release memory from a nullptr.");

            auto key = getKeyFromValue(value);
            PR_CORE_ASSERT(key != -1, "A valid key was not found for value {}",
                           fmt::ptr(value));

            this->release(key);
        }

        bool isAllocated(ValueType search_value) {
            for (auto const& [key, value] : _entries) {
                if (value == search_value) return true;
            }

            return false;
        }

        [[nodiscard]] id_t getNextId() const { return this->_currentId; }

       private:
        // const bool _useNestedID;

        id_t getKeyFromValue(ValueType search_value) {
            for (auto const& [key, value] : _entries) {
                if (value == search_value) return key;
            }

            return -1;
        }

        std::map<id_t, ValueType> _entries;
        // InstantiationFunction _instantiator;
        id_t _currentId;
    };
}  // namespace Presto
