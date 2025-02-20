#pragma once

#include "Presto/Core/Types.h"
namespace Presto {

class PRESTO_API Buffer {
   public:
    enum class BufferType : Presto::uint8_t { VERTEX, INDEX, UNIFORM };

    [[nodiscard]] Presto::size_t size() const { return size_; }
    [[nodiscard]] BufferType type() const { return type_; };

    virtual void write(const ByteArray& bytes, Presto::size_t offset = 0) = 0;

    virtual void bind() = 0;

    virtual ~Buffer() = default;

    Buffer& operator=(Buffer&&) = default;
    Buffer(Buffer&&) = delete;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

   protected:
    explicit Buffer(BufferType type, Presto::size_t size)
        : type_(type), size_(size) {};

   private:
    BufferType type_;
    Presto::size_t size_{};
};

}  // namespace Presto
