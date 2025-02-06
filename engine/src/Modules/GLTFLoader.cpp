#include "GLTFLoader.h"

#include "tiny_gltf.h"

// NOT NEEDED SINCE TINY_GLTF USES IT
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Presto {

struct AccessorData {
    ByteArray buffer;
    size_t count{static_cast<size_t>(-1)};
};

AccessorData getDataFromAccessor(const tinygltf::Model& model,
                                 size_t accessorIndex) {
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    IndexList indices(accessor.count);

    ByteArray ret_buffer(bv.byteLength);

    auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    std::memcpy(ret_buffer.data(), buffer.data.data() + accessor_offset,
                bv.byteLength);

    AccessorData ret{.buffer = ret_buffer, .count = accessor.count};

    return ret;
}

template <typename T>
std::vector<T> getDataFromAccessor2(const tinygltf::Model& model,
                                    size_t accessorIndex) {
    constexpr size_t read_size = sizeof(T);

    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    const auto read_count = accessor.count;

    std::vector<T> ret(read_count);

    auto accessor_offset = bv.byteOffset + accessor.byteOffset;
    auto stride = bv.byteStride;
    if (stride == 0) {
        stride = read_size;
    };

    // for (int byte = accessor_offset; byte < read_count; byte++) {
    for (size_t i = 0; i < read_count; i++) {
        std::memcpy(&ret[i], &buffer.data[accessor_offset + (i * stride)],
                    read_size);
    }

    return ret;
}

template <typename T>
std::vector<T> castBuffer(const ByteArray& bytes) {
    constexpr size_t read_size = sizeof(T);

    auto read_count = bytes.size() / read_size;
    std::vector<T> ret(read_count);

    for (size_t i = 0; i < read_count; i++) {
        std::memcpy(&ret[i], &bytes[i * read_size], read_size);
    }

    return ret;
}

}  // namespace Presto
