#pragma once

namespace Presto {

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

}  // namespace Presto
