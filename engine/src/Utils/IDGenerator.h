#include <concepts>
#include <type_traits>
#include <unordered_set>

namespace Presto {

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
    explicit IDGenerator(T threshold = 1) : currentId_(minGeneratedThreshold_) {
        PR_CORE_ASSERT(threshold > 0,
                       "IDGenerator can't be instantiated with a value of 0, "
                       "as this is reserved for INVALID_ID");
        minGeneratedThreshold_ = threshold;
    }

    ~IDGenerator() = default;

    /**
     * @brief	Reserves an unused ID and returns it. This should generally be
     *used rather than reserve.
     * @return	The newly reserved ID.
     */
    T generate() { return currentId_++; };

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

    IDGenerator(const IDGenerator&) = delete;
    IDGenerator(IDGenerator&&) = delete;
    IDGenerator& operator=(const IDGenerator&) = delete;
    IDGenerator& operator=(IDGenerator&&) = delete;

   private:
    T minGeneratedThreshold_{1};

    T currentId_{1};

    std::unordered_set<T> ids_;

    // TODO: Implement freeing IDs, as well as a queue of freedIds_
    // std::queue<T> freedIds_;
};
}  // namespace Presto
