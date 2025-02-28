#pragma once

namespace Presto {

class UniformBuffer {
   public:
    virtual void write(const std::span<std::byte>& bytes,
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
