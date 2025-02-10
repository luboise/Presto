#pragma once

#include <type_traits>
#include "Presto/Core/Types.h"

#include "Presto/Rendering/UniformTypes.h"

namespace Presto {
class ErasedBytes {
   public:
    ErasedBytes() { setData(Presto::float32_t{0}); };

    template <typename T>
    explicit ErasedBytes(T val) {
        this->setData(std::forward<T>(val));
    }

    /*template <typename T>*/
    /*    requires std::is_copy_constructible_v<T> &&
     * std::is_copy_assignable_v<T>*/
    /*explicit ErasedBytes(T val) {*/
    /*    this->setData(std::forward<T>(val));*/
    /*};*/

    template <typename T>
        requires std::is_copy_constructible_v<T>
    void setData(T newData) {
        auto new_size{sizeof(newData)};

        data_.resize(new_size);
        std::memcpy(data_.data(), &newData, new_size);
    }

    template <typename T>
        requires std::ranges::range<T>
    void setData(T newData) {
        auto new_size{sizeof(newData.first()) * newData.size()};

        std::memcpy(data_.data(), newData.data(), new_size);
    };

    template <typename T>
    [[nodiscard]] T as() const {
        if (data_.size() != sizeof(T)) {
            // TODO: Get rid of this exception and do something more safe
            throw std::invalid_argument(
                "Size of vector does not match size of target type.");
        }

        return std::bit_cast<T>(data_);
    }

    template <UniformVariableType T>
    [[nodiscard]] typename UniformVariableTypeTraits<T>::type as() const {
        using return_t = UniformVariableTypeTraits<T>::type;
        return as<return_t>();
    }

    std::span<std::byte> getData() { return data_; }

   private:
    ByteArray data_{4, std::byte{0}};
};
}  // namespace Presto
