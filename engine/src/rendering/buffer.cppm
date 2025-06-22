export module presto.internal.rendering:buffer;

import presto.core.types;
import presto.utils;

export namespace Presto {

class Buffer {
   public:
    using buffer_write_t = std::span<const std::byte>;

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

class UniformBuffer {
   public:
    template <typename T>
        requires requires { sizeof(T); }
    void writeAny(const T& val, Presto::size_t offset = 0) {
        this->write(
            std::span(reinterpret_cast<const std::byte*>(&val), sizeof(T)),
            std::move(offset));
    }

    virtual void write(const std::span<const std::byte>& bytes,
                       Presto::size_t offset = 0) = 0;

    virtual void bind(Presto::size_t index) = 0;
    virtual void unbind() = 0;

    [[nodiscard]] Presto::size_t size() const { return size_; }

    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer& operator=(UniformBuffer&&) = delete;

    virtual ~UniformBuffer() = default;

   protected:
    explicit UniformBuffer(Presto::size_t size) : size_(size) {};

   private:
    Presto::size_t size_;
};

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
