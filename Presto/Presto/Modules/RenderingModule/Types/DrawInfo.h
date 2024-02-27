namespace Presto {
    typedef std::vector<uint32_t> IndexList;

    struct PRESTO_API DrawInfo {
        IndexList indices;

        uint32_t vbOffset;
        int32_t ibOffset;
    };
}  // namespace Presto
