#include "GLTFLoader.h"
#include <type_traits>

#include "Presto/Assets/AssetPath.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Types.h"
#include "Presto/Rendering/AttributeTypes.h"
#include "tiny_gltf.h"

// NOT NEEDED SINCE TINY_GLTF USES IT
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Presto/Assets/ModelAsset.h"

namespace Presto {

ShaderDataType tinygltfToPrestoType(const tinygltf::Accessor& accessor) {
    switch (accessor.type) {
        case TINYGLTF_TYPE_VEC2: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::VEC2;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DVEC2;
            }
        }
        case TINYGLTF_TYPE_VEC3: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::VEC3;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DVEC3;
            }
        }
        case TINYGLTF_TYPE_VEC4: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::VEC4;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DVEC4;
            }
        }
        case TINYGLTF_TYPE_MAT4: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::MAT4;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DMAT4;
            }
        }
        case TINYGLTF_TYPE_SCALAR: {
            switch (accessor.componentType) {
                case (TINYGLTF_COMPONENT_TYPE_FLOAT):
                    return ShaderDataType::FLOAT;
                case (TINYGLTF_COMPONENT_TYPE_DOUBLE):
                    return ShaderDataType::DOUBLE;
                case (TINYGLTF_COMPONENT_TYPE_INT):
                    return ShaderDataType::INT;
                case (TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT):
                    return ShaderDataType::UINT;
                default: {
                    PR_ASSERT(false, "Invalid data type import attempted.");
                    return ShaderDataType::FLOAT;
                }
            }
        }
        default: {
            PR_ASSERT(false, "Invalid data type import attempted.");
            return ShaderDataType::FLOAT;
        }
    }
};

PR_STRING_ID tinygltfNameToPrestoName(const PR_STRING_ID& name) {
    if (name == "POSITION") {
        return DefaultAttributeName::POSITION;
    }
    if (name == "NORMAL") {
        return DefaultAttributeName::NORMAL;
    }
    if (name == "TEXCOORD_0") {
        return DefaultAttributeName::TEXCOORDS;
    }

    return name;
}

ImportedAttribute getDataFromAccessor(const tinygltf::Model& model,
                                      size_t accessorIndex) {
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
    ShaderDataType type{tinygltfToPrestoType(accessor)};

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    IndexList indices(accessor.count);

    ByteArray ret_buffer(bv.byteLength);

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    std::memcpy(ret_buffer.data(), buffer.data.data() + accessor_offset,
                bv.byteLength);

    ImportedAttribute ret{
        .name = accessor.name,
        .type = type,
        .count = accessor.count,
        .data = ret_buffer,
    };

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

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;
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

template <typename T>
    requires std::is_floating_point_v<T>
Presto::vec4 vec4FromVector(std::vector<T> vec) {
    PR_ASSERT(vec.size() == 4,
              std::format("Invalid vector size: ", vec.size()));

    return {
        vec[0],
        vec[1],
        vec[2],
        vec[3],
    };
}

ImportedModelData GLTFLoader::load(
    const AssetPath& path, const std::vector<asset_name_t>& customNames) {
    FilePath filename{path.stem()};
    FilePath file_extension = path.extension();

    ImportedModelData imports{};

    if (file_extension != ".gltf" && file_extension != ".glb") {
        // TODO: Make this return an error or print it instead
        return imports;
    }

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    // TODO: Implement full path/cwd system for engine to find it at
    // runtime, or have the user change it (would help the editor)
    // FilePath full_asset_path =
    // Utils::File::getFullPath(filepath);
    const FilePath& full_asset_path = path;

    bool ret{false};

    if (file_extension == ".gltf") {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, full_asset_path);
    } else {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, full_asset_path);
    }

    PR_CORE_ASSERT(
        ret, std::string("Failed to read asset ") + full_asset_path.string());

    Presto::size_t binding{0};

    for (auto& material : model.materials) {
        MaterialStructure structure;

        ImportedMaterial new_material{.name = material.name,
                                      .structure = std::move(structure)};

        Presto::vec4 val =
            vec4FromVector(material.pbrMetallicRoughness.baseColorFactor);

        structure.properties.push_back(
            DefaultMaterialProperties::BaseColour(val));

        if (auto index{material.pbrMetallicRoughness.baseColorTexture.index};
            index != -1) {
            structure.properties.push_back(
                DefaultMaterialProperties::DiffuseTexture(index));

            const auto& texture = model.textures[index];
            const auto& image_data = model.images[texture.source];

            Presto::Image image{
                .width = static_cast<size_t>(image_data.width),
                .height = static_cast<size_t>(image_data.height),
                .bytes{}};

            image.bytes.resize(image.size());

            std::memcpy(image.bytes.data(), image_data.image.data(),
                        image.bytes.size());

            if (imports.images.size() <= (index - 1U)) {
                imports.images.resize(index);
            }

            imports.images[index] = image;
        }
    }

    for (size_t i = 0; i < model.meshes.size(); i++) {
        auto& current_model = model.meshes[i];

        ImportedModel new_model{};

        new_model.name =
            i >= (customNames.size() - 1) ? customNames[i] : current_model.name;

        for (tinygltf::Primitive& mesh : current_model.primitives) {
            ImportedMesh new_submesh;

            new_submesh.draw_mode = mesh.mode;

            new_submesh.indices = getDataFromAccessor2<RawMeshData::IndexType>(
                model, mesh.indices);

            new_submesh.material = mesh.material;

            std::vector<ImportedAttribute> imported_attributes;

            for (const auto& [attribute, accessor_index] : mesh.attributes) {
                auto accessor{model.accessors[accessor_index]};
                auto data{getDataFromAccessor(model, accessor_index)};

                data.name = tinygltfNameToPrestoName(data.name);

                imported_attributes.push_back(data);
            }

            new_submesh.attributes = imported_attributes;
            new_model.meshes.push_back(new_submesh);
        }

        imports.models.push_back(new_model);
    }

    return imports;
};

}  // namespace Presto
