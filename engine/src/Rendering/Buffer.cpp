#include "Buffer.h"

namespace Presto {

Buffer::Buffer(BufferType type, Presto::size_t size)
    : type_(type), size_(size) {};

Presto::size_t Buffer::getWriteSize(Presto::size_t desiredSize,
                                    Presto::size_t startOffset) const {
    Presto::size_t write_size{std::min(desiredSize, this->size_ - startOffset)};

    if (write_size != desiredSize) {
        PR_WARN(
            "Requested write of size {} to Buffer of size {} exceeds the "
            "boundaries "
            "of "
            "the buffer by {} bytes. Writing only up until the end of the "
            "buffer.",
            desiredSize, this->size_, desiredSize - write_size);
    }

    return write_size;
}

Buffer::BufferType Buffer::type() const { return type_; };
Presto::size_t Buffer::size() const { return size_; }

}  // namespace Presto
