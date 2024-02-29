namespace Presto {
    typedef std::vector<uint32_t> IndexList;

    struct PRESTO_API DrawInfo {
        IndexList indices;

        uint32_t vbOffset = 0;
        int32_t ibOffset = 0;
    };
}  // namespace Presto
