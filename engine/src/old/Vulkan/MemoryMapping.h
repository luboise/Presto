namespace Pr {

class MemoryMapping {
   public:
    MemoryMapping(Pr::uint32_t offset, Pr::uint32_t size);
    ~MemoryMapping();

    operator std::span<std::byte>() const;

    [[nodiscard]] std::byte* data() const {
        return reinterpret_cast<std::byte*>(mapping_);
    }

   private:
    void* mapping_;
};

}  // namespace Pr
