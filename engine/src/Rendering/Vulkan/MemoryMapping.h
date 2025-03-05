namespace Presto {

class MemoryMapping {
   public:
    MemoryMapping(Presto::uint32_t offset, Presto::uint32_t size);
    ~MemoryMapping();

    operator std::span<std::byte>() const;

    [[nodiscard]] std::byte* data() const {
        return reinterpret_cast<std::byte*>(mapping_);
    }

   private:
    void* mapping_;
};

}  // namespace Presto
