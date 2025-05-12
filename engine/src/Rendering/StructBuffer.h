#pragma once

#include <type_traits>
#include "Presto/Rendering/UniformBuffer.h"
#include "Presto/Utils/LazyCalculator.h"

#include "Modules/RenderingManager.h"

namespace Presto {

template <class T>
    requires std::is_trivially_move_constructible_v<T>
class StructUniformBuffer : LazyCalculator {
   public:
    explicit StructUniformBuffer(T data = {}) : data_(data), LazyCalculator() {
        buffer_ = RenderingManager::get().createUniformBuffer(sizeof(T));
    }

    UniformBuffer& buffer() {
        if (this->isDirty()) {
            buffer_->writeAny(data_);
            setDirty(false);
        }

        return *buffer_;
    }

    [[nodiscard]] const auto& data() const { return data_; }

    void write(T&& data) {
        data_ = std::move(data);
        setDirty();
    };

    void write(T& data) {
        data_ = data;
        setDirty();
    };

   private:
    T data_{};
    Allocated<UniformBuffer> buffer_;
};
}  // namespace Presto
