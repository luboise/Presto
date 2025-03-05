#pragma once

#include "Presto/Core/Logging.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Types/CoreTypes.h"

#include <array>
#include <cstring>
#include <span>
#include <type_traits>

namespace Presto {
class ErasedBytes {
   public:
    ErasedBytes() { reset(Presto::float32_t{0}); };

    template <typename T>
    explicit ErasedBytes(T val) {
        this->reset(std::forward<T>(val));
    }

    /*template <typename T>*/
    /*    requires std::is_copy_constructible_v<T> &&
     * std::is_copy_assignable_v<T>*/
    /*explicit ErasedBytes(T val) {*/
    /*    this->setData(std::forward<T>(val));*/
    /*};*/

    [[nodiscard]] Presto::size_t size() const { return data_.size(); }

    template <typename T>
        requires std::is_copy_constructible_v<T> && (!std::ranges::range<T>)
    void reset(T newData) {
        auto new_size{sizeof(newData)};
        data_.resize(new_size);

        this->forceWrite(newData);
    }

    template <typename T>
        requires std::ranges::range<T>
    void reset(T newData) {
        auto new_size{sizeof(T) * newData.size()};
        data_.resize(new_size);

        this->forceWrite(newData);
    };

    // TODO: Double check memory semantics here
    template <typename T>
        requires std::is_copy_constructible_v<T>
    void write(T&& newData) {
        this->write(std::forward<const T&>(newData));
    };

    template <typename T>
        requires std::is_copy_constructible_v<T>
    void write(T& newData) {
        auto data_size{sizeof(newData)};

        if (data_size != data_.size()) {
            PR_ERROR(
                "Mismatch between current size of ErasedBytes ({}) and size of "
                "input data {}. Skipping this write.",
                data_.size(), data_size);
            return;
        }

        this->forceWrite(newData);
    };

    template <typename T>
        requires std::ranges::range<T>
    void write(T& newData) {
        auto data_size{sizeof(newData.first()) * newData.size()};

        if (data_size != data_.size()) {
            PR_ERROR(
                "Mismatch between current size of ErasedBytes ({}) and size of "
                "input data {}. Skipping this write.",
                data_.size(), data_size);
            return;
        }

        this->forceWrite(newData);
    };

    template <typename T>
    [[nodiscard]] T as() const {
        if (data_.size() != sizeof(T)) {
            // TODO: Get rid of this exception and do something more safe
            throw std::invalid_argument(
                "Size of vector does not match size of target type.");
        }

        std::array<std::byte, sizeof(T)> byte_array;
        std::ranges::copy_n(data_.begin(), sizeof(T), byte_array.begin());

        return std::bit_cast<T>(byte_array);
    }

    [[nodiscard]] ByteArray bytes() const { return this->data_; }

    /*
template <UniformVariableType T>
[[nodiscard]] typename UniformVariableTypeTraits<T>::type as() const {
    using return_t = UniformVariableTypeTraits<T>::type;
    return as<return_t>();
}
    */

    std::span<std::byte> getData() { return data_; }

   private:
    ByteArray data_{4, std::byte{0}};

    /**
     * @brief  Write without checking the size of the data.
     */
    template <typename T>
        requires std::is_copy_constructible_v<T> && (!std::ranges::range<T>)
    void forceWrite(T newData) {
        std::memcpy(data_.data(), &newData, data_.size());
    }

    template <typename T>
        requires std::ranges::range<T>
    void forceWrite(T& newData) {
        std::memcpy(data_.data(), newData.data(), data_.size());
    };
};

}  // namespace Presto
