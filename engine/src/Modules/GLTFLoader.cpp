#include "GLTFLoader.h"

#include <algorithm>
#include <bit>
#include <set>
#include <type_traits>

#include "Presto/Assets/AssetPath.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/ShaderTypes.h"
#include "Presto/Rendering/UniformTypes.h"
#include "Presto/Types/CoreTypeTraits.h"
#include "Presto/Types/CoreTypes.h"
#include "tiny_gltf.h"

// NOT NEEDED SINCE TINY_GLTF USES IT
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
            break;
        }
        case TINYGLTF_TYPE_VEC3: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::VEC3;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DVEC3;
            }
            break;
        }
        case TINYGLTF_TYPE_VEC4: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::VEC4;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DVEC4;
            }
            break;
        }
        case TINYGLTF_TYPE_MAT4: {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                return ShaderDataType::MAT4;
            }
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_DOUBLE) {
                return ShaderDataType::DMAT4;
            }
            break;
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
                default:
                    break;
            }
            break;
        }
    }

    PR_ASSERT(false, "Invalid data type import attempted.");
    return ShaderDataType::FLOAT;
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

ImportedVertexAttribute getDataFromAccessor(const tinygltf::Model& model,
                                            size_t accessorIndex) {
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
    ShaderDataType type{tinygltfToPrestoType(accessor)};

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    IndexList indices(accessor.count);

    ByteArray ret_buffer(bv.byteLength);

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    std::memcpy(ret_buffer.data(), &buffer.data[accessor_offset],
                bv.byteLength);

    ImportedVertexAttribute ret{
        .name = accessor.name,
        .type = type,
        .count = accessor.count,
        .data = ret_buffer,
    };

    return ret;
}

/*
template <typename T>
std::vector<T> getDataFromAccessor2(const tinygltf::Model& model,
                                    size_t accessorIndex) {
    constexpr size_t read_size = sizeof(T);

    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    const auto read_count = accessor.count;

    std::vector<T> ret(read_count);

    auto innerFunction{[]<typename InnerT>() {

        }};

    // Find original data type
    using ogdatatype = uint16_t;

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    // Get the stride (it must be at least the size of the data)
    auto stride = std::max(bv.byteStride, read_size);
    ogdatatype value{};

    std::array<unsigned char, sizeof(ogdatatype)> myarray{};
    for (size_t i = 0; i < read_count; i++) {
        // Get the amount of bytes of the original data type
        std::ranges::copy_n(&buffer.data[accessor_offset + (i * stride)],
                            sizeof(ogdatatype), myarray.begin());

        value = std::bit_cast<ogdatatype>(myarray);
        ret[i] = static_cast<T>(value);
    }

    return ret;
}
   */

template <typename T>
[[nodiscard]] std::vector<T> getAccessorAs(const tinygltf::Model& model,
                                           Presto::size_t accessorIndex) {
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];

    const auto& bv = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bv.buffer];

    const auto read_count = accessor.count;
    std::vector<T> ret(read_count);

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    // Get the stride (it must be at least the size of the data)
    auto stride = std::max(bv.byteStride, sizeof(T));

    std::array<unsigned char, sizeof(T)> bytes{};
    for (size_t i = 0; i < read_count; i++) {
        // Get the amount of bytes of the original data type
        std::ranges::copy_n(&buffer.data[accessor_offset + (i * stride)],
                            sizeof(T), bytes.begin());

        ret[i] = std::bit_cast<T>(bytes);
    }

    return ret;
};

/*
template <ShaderDataType TO_T, ShaderDataType FROM_T>
bool getAccessorCharsBySubtype(const tinygltf::Model& model,
                               Presto::size_t accessorIndex,
                               std::vector<unsigned char>& outChars) {
    // Get the data as the gltf subtype
    using TinyGLTFSubtype = SubTypeOf<ShaderImportTypeOf<FROM_T>>;
    std::vector<TinyGLTFSubtype> data_as_tinygltf_subtype{
        getAccessorAs<TinyGLTFSubtype>(model, accessorIndex)};

    using DesiredSubtype = SubTypeOf<ShaderImportTypeOf<TO_T>>;

    // Convert it to the type we want std::vector<SubTypeOf<T>>
    std::vector<DesiredSubtype> subtype_data(data_as_tinygltf_subtype.begin(),
                                             data_as_tinygltf_subtype.end());

    using Wanted = SubTypeOf<ShaderImportTypeOf<GLTFType>>;

    outChars.resize(subtype_data.size() * sizeof(SubTypeOf<T>));

    std::memcpy(outChars.data(), subtype_data.data(), outChars.size());

    return true;
}
*/

template <typename T>
std::vector<T> getAccessorAndConvertTo(const tinygltf::Model& model,
                                       size_t accessorIndex) {
    // Find original data type

    ShaderDataType tinygltf_type{
        tinygltfToPrestoType(model.accessors[accessorIndex])};

    std::vector<unsigned char> chars;
    std::vector<T> ret;

    auto vector_to_uchars{
        []<typename J>(std::vector<J> inVec) -> std::vector<unsigned char> {
            std::vector<unsigned char> chars(inVec.size() * sizeof(J));

            std::memcpy(chars.data(), inVec.data(), chars.size());
            return chars;
        }};

#define SWITCH_CASE(type)                                                      \
    chars =                                                                    \
        vector_to_uchars(getAccessorAs<SubTypeOf<ShaderImportTypeOf<(type)>>>( \
            model, accessorIndex));

    switch (tinygltf_type) {
        SWITCH_CASE(ShaderDataType::INT)
        SWITCH_CASE(ShaderDataType::UINT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC3);
        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::MAT3);
        SWITCH_CASE(ShaderDataType::MAT4);
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::FLOAT);
        default: {
            PR_ERROR("Unable to parse data.");
            return {};
        }
    }

#undef SWITCH_CASE

    ret.resize(chars.size() / sizeof(T));

    std::span<unsigned char> char_view{chars};

    std ::array<unsigned char, sizeof(T)> bytes{};
    for (size_t i = 0; i < ret.size(); i++) {
        std ::memcpy(bytes.data(), &char_view[i * sizeof(T)], sizeof(T));
        ret[i] = std ::bit_cast<T>(bytes);
    }

    return ret;

}  // namespace Presto

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

ImportedMaterial importMaterialFromGLTF(const tinygltf::Material& material,
                                        std::set<int>& texturesToLoad) {
    ImportedMaterial imported;

    const auto& pbr{material.pbrMetallicRoughness};

    tinygltf::PbrMetallicRoughness basePbr{};

    if (pbr.baseColorFactor != basePbr.baseColorFactor) {
        imported.values.push_back(ImportedMaterialProperty{
            .name = DefaultMaterialPropertyName::BASE_COLOUR,
            .data_type = UniformVariableType::VEC4,
            .data = ErasedBytes{pbr.baseColorFactor}});
    }

    if (auto index{pbr.baseColorTexture.index}; index != -1) {
        texturesToLoad.emplace(index);

        imported.values.push_back(ImportedMaterialProperty{
            .name = DefaultMaterialPropertyName::DIFFUSE_TEXTURE,
            .data_type = UniformVariableType::TEXTURE,
            .data = ErasedBytes{
                static_cast<ImportTypeOf<UniformVariableType::TEXTURE>>(
                    index)}});
    }

    return imported;
}

ImportedTexture importTextureFromGLTF(const tinygltf::Model& model,
                                      const int index) {
    PR_CORE_ASSERT(model.images.size() >= index + 1U,
                   "Invalid GLTF processing has occurred, even though it "
                   "should have been validated.");
    const auto& image_data = model.images[index];

    Presto::Image image{.width = static_cast<size_t>(image_data.width),
                        .height = static_cast<size_t>(image_data.height),
                        .bytes{}};

    image.bytes.resize(image.size());
    std::memcpy(image.bytes.data(), image_data.image.data(),
                image.bytes.size());

    Presto::string new_name{};
    if (!image_data.name.empty()) {
        new_name = image_data.name;

    } else if (!image_data.uri.empty()) {
        new_name = image_data.uri;
    } else {
        PR_ERROR(
            "Name and URI are both empty inside of an imported GLTF texture. "
            "Using \"UnnamedImportedTexture\" instead.");
        new_name = "UnnamedImportedTexture";
    }

    return {.name = new_name, .image = std::move(image)};
}

ImportedModelData GLTFLoader::load(
    const AssetPath& path, const std::vector<asset_name_t>& customNames) {
    FilePath filename{path.basename()};
    FilePath file_extension = path.fileExtension();

    ImportedModelData imported_data{};

    if (file_extension != ".gltf" && file_extension != ".glb") {
        // TODO: Make this return an error or print it instead
        return imported_data;
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

    std::set<int> textureIndices;

    for (tinygltf::Material& material : model.materials) {
        imported_data.materials.push_back(
            importMaterialFromGLTF(material, textureIndices));
    }

    if (textureIndices.size() > 0) {
        imported_data.textures.resize(std::ranges::max(textureIndices) + 1);

        for (const auto& index : textureIndices) {
            imported_data.textures[index] = importTextureFromGLTF(model, index);
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

            new_submesh.indices =
                getAccessorAndConvertTo<Index>(model, mesh.indices);

            new_submesh.material_index = mesh.material;

            std::vector<ImportedVertexAttribute> imported_attributes;

            for (const auto& [attribute_name, accessor_index] :
                 mesh.attributes) {
                auto accessor{model.accessors[accessor_index]};
                auto data{getDataFromAccessor(model, accessor_index)};

                data.name = tinygltfNameToPrestoName(attribute_name);

                imported_attributes.push_back(data);
            }

            new_submesh.attributes = imported_attributes;
            new_model.meshes.push_back(new_submesh);
        }

        imported_data.models.push_back(new_model);
    }

    return imported_data;
};

}  // namespace Presto
