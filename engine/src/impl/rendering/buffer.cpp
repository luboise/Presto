module presto.rendering.buffer;

import presto.core.logging;

namespace Pr {

Buffer::Buffer(BufferType type, Pr::size_t size) : type_(type), size_(size) {};

Pr::size_t Buffer::getWriteSize(Pr::size_t desiredSize,
                                Pr::size_t startOffset) const {
    Pr::size_t write_size{std::min(desiredSize, this->size_ - startOffset)};

    if (write_size != desiredSize) {
        Pr::CoreLog(
            WARN,
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
Pr::size_t Buffer::size() const { return size_; }

}  // namespace Pr
