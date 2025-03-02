#pragma once

#include "Presto/Types/CoreTypes.h"

#include <span>

namespace Presto {

class PRESTO_API Buffer {
   public:
    using buffer_write_t = std::span<std::byte>;

    enum class BufferType : Presto::uint8_t { VERTEX, INDEX, UNIFORM };

    [[nodiscard]] Presto::size_t size() const;
    [[nodiscard]] BufferType type() const;

    virtual void write(buffer_write_t bytes, Presto::size_t offset = 0) = 0;

    virtual void bind() = 0;

    virtual ~Buffer() = default;

    Buffer& operator=(Buffer&&) = default;
    Buffer(Buffer&&) = delete;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    [[nodiscard]] Presto::size_t getWriteSize(Presto::size_t desiredSize,
                                              Presto::size_t startOffset) const;

   protected:
    explicit Buffer(BufferType type, Presto::size_t size);

   private:
    BufferType type_;
    Presto::size_t size_{};
};

}  // namespace Presto
