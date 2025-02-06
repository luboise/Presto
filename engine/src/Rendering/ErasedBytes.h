#pragma once

#include <type_traits>
namespace Presto {
class ErasedBytes {
   public:
    template <typename T>
    ErasedBytes(T&& val) {
        this->setData(val);
    };

    template <typename T>
        requires std::is_copy_constructible_v<T>
    void setData(T newData) {
        auto new_size{sizeof(newData)};

        data_.resize(new_size);
        std::memcpy(data_.data(), newData, new_size);
    }

    template <typename T>
        requires std::is_move_assignable_v<T>
    void setData(T&& newData) {
        data_ = std::move(newData);

        std::memcpy(data_.data(), newData, new_size);
    }

    template <typename T>
        requires std::ranges::range<T>
    void setData(T newData) {
        auto new_size{sizeof(newData.first()) * newData.size()};

        std::memcpy(data_.data(), newData.data(), new_size);
    };

    template <typename T>
    T getData() {
        if (data_.size() != sizeof(T)) {
            // TODO: Get rid of this exception and do something more safe
            throw std::invalid_argument(
                "Size of vector does not match size of target type.");
        }

        return std::bit_cast<T>(data_);
    }

    std::span<std::byte> getData() { return data_; }

   private:
    ByteArray data_{ByteArray(4, std::byte{0})};
};
}  // namespace Presto
