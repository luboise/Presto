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
                case (TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT):
                    return ShaderDataType::USHORT;
                case (TINYGLTF_COMPONENT_TYPE_SHORT):
                    return ShaderDataType::SHORT;
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

ImportedVertexAttribute oldGetAttributeFromAccessor(
    const tinygltf::Model& model, size_t accessorIndex) {
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
        .data{},
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

    // const auto read_count = bv.byteLength / sizeof(T);
    const auto read_count =
        accessor.count * tinygltf::GetNumComponentsInType(accessor.type);

    std::vector<T> ret(read_count);

    const auto accessor_offset = bv.byteOffset + accessor.byteOffset;

    // Get the stride (it must be at least the size of the data)
    // auto stride = std::max(bv.byteStride, sizeof(T));

    std::span<const T> span{
        reinterpret_cast<const T*>(&buffer.data[accessor_offset]), read_count};

    for (size_t i = 0; i < read_count; i++) {
        ret[i] = span[i];
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

    auto vector_to_uchars{
        []<typename J>(std::vector<J> inVec) -> std::vector<unsigned char> {
            std::vector<unsigned char> chars(inVec.size() * sizeof(J));

            std::memcpy(chars.data(), inVec.data(), chars.size());
            return chars;
        }};


    return true;
}
*/

ImportedVertexAttribute getAttributeFromAccessor(const tinygltf::Model& model,
                                                 Presto::size_t accessorIndex) {
    const auto& accessor{model.accessors[accessorIndex]};
    const auto& bufferview = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferview.buffer];

    auto converted_name{tinygltfNameToPrestoName(accessor.name)};
    auto type{tinygltfToPrestoType(accessor)};

    const std::span<const std::byte> bytes{
        reinterpret_cast<const std::byte*>(
            &buffer.data[bufferview.byteOffset + accessor.byteOffset]),
        accessor.count * SizeOfShaderType(type)};

    return {.name = converted_name,
            .type = tinygltfToPrestoType(accessor),
            .count = accessor.count,
            .data = ByteArray(bytes.begin(), bytes.end())};
}

const auto vector_to_uchars{
    []<typename J>(std::vector<J> inVec) -> std::vector<unsigned char> {
        std::vector<unsigned char> chars(inVec.size() * sizeof(J));

        std::memcpy(chars.data(), inVec.data(), chars.size());
        return chars;
    }};

template <typename T>
std::vector<T> getAccessorAndConvertTo(const tinygltf::Model& model,
                                       size_t accessorIndex) {
    // Find original data type

    const auto& accessor{model.accessors[accessorIndex]};

    ShaderDataType tinygltf_type{tinygltfToPrestoType(accessor)};

    std::vector<unsigned char> chars;
    std::vector<T> ret;

#define SWITCH_CASE(type)                                                 \
    case (type): {                                                        \
        auto original_data{                                               \
            getAccessorAs<SubTypeOf<ShaderImportTypeOf<(type)>>>(         \
                model, accessorIndex)};                                   \
        std::vector<SubTypeOf<T>> substituted_data{original_data.begin(), \
                                                   original_data.end()};  \
        chars = vector_to_uchars(std::move(substituted_data));            \
        break;                                                            \
    }

    switch (tinygltf_type) {
        SWITCH_CASE(ShaderDataType::INT)
        SWITCH_CASE(ShaderDataType::UINT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC2);
        case (ShaderDataType ::VEC3): {
            auto original_data{getAccessorAs<
                SubTypeOf<ShaderImportTypeOf<(ShaderDataType ::VEC3)>>>(
                model, accessorIndex)};
            std ::vector<SubTypeOf<T>> substituted_data{original_data.begin(),
                                                        original_data.end()};
            chars = vector_to_uchars(std ::move(substituted_data));
            break;
        };
            SWITCH_CASE(ShaderDataType::VEC4);
            SWITCH_CASE(ShaderDataType::MAT3);
            SWITCH_CASE(ShaderDataType::MAT4);
            SWITCH_CASE(ShaderDataType::SHORT);
        case (ShaderDataType ::USHORT): {
            auto original_data{getAccessorAs<
                SubTypeOf<ShaderImportTypeOf<(ShaderDataType ::USHORT)>>>(
                model, accessorIndex)};
            std ::vector<SubTypeOf<T>> substituted_data{original_data.begin(),
                                                        original_data.end()};
            chars = vector_to_uchars(std ::move(substituted_data));
            break;
        };
            SWITCH_CASE(ShaderDataType::FLOAT);
        default: {
            PR_ERROR("Unable to parse data.");
            return {};
        }
    }

#undef SWITCH_CASE

    ret.resize(accessor.count);

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
            "Name and URI are both empty inside of an imported GLTF "
            "texture. "
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

            switch (mesh.mode) {
                case TINYGLTF_MODE_POINTS:
                    new_submesh.draw_mode = MeshDrawMode::POINTS;
                case TINYGLTF_MODE_LINE:
                    new_submesh.draw_mode = MeshDrawMode::LINES;
                case TINYGLTF_MODE_LINE_STRIP:
                    new_submesh.draw_mode = MeshDrawMode::LINE_STRIP;
                case TINYGLTF_MODE_TRIANGLES:
                    new_submesh.draw_mode = MeshDrawMode::TRIANGLES;
                case TINYGLTF_MODE_TRIANGLE_STRIP:
                    new_submesh.draw_mode = MeshDrawMode::TRIANGLE_STRIP;
                case TINYGLTF_MODE_LINE_LOOP:
                case TINYGLTF_MODE_TRIANGLE_FAN:
                default:
                    new_submesh.draw_mode = MeshDrawMode::TRIANGLES;
            }

            new_submesh.indices =
                getAccessorAndConvertTo<Index>(model, mesh.indices);

            new_submesh.material_index = mesh.material;

            std::vector<ImportedVertexAttribute> imported_attributes;

            for (const auto& [attribute_name, accessor_index] :
                 mesh.attributes) {
                auto converted_name{tinygltfNameToPrestoName(attribute_name)};

                if (converted_name == DefaultAttributeName::POSITION ||
                    converted_name == DefaultAttributeName::NORMAL ||
                    converted_name == DefaultAttributeName::COLOUR) {
                    auto data{getAccessorAs<Presto::float32_t>(model,
                                                               accessor_index)};

                    ImportedVertexAttribute new_attribute{
                        .name = converted_name,
                        .type = ShaderDataType::VEC3,
                        .count = model.accessors[accessor_index].count,
                        .data =
                            ByteArray(data.size() * sizeof(Presto::float32_t))};

                    std::memcpy(new_attribute.data.data(), data.data(),
                                new_attribute.data.size());
                    imported_attributes.push_back(std::move(new_attribute));

                } else if (converted_name == DefaultAttributeName::TEXCOORDS) {
                    auto data{
                        getAccessorAs<Presto::vec2>(model, accessor_index)};

                    ImportedVertexAttribute new_attribute{
                        .name = converted_name,
                        .type = ShaderDataType::VEC2,
                        .count = model.accessors[accessor_index].count,
                        .data = ByteArray(data.size() * sizeof(Presto::vec2))};

                    std::memcpy(new_attribute.data.data(), data.data(),
                                new_attribute.data.size());
                    imported_attributes.push_back(std::move(new_attribute));

                } else {
                    PR_INFO(
                        "{} is not a default attribute. Skipping this in "
                        "GLTF "
                        "import.",
                        attribute_name);
                    continue;
                }
            }

            new_submesh.attributes = std::move(imported_attributes);

            // The vertex count is the minimum of the vertex counts of the
            // attributes
            new_submesh.vertex_count = std::ranges::min(
                new_submesh.attributes |
                std::views::transform(
                    [](const ImportedVertexAttribute& val) -> Presto::size_t {
                        return val.count;
                    }));

            new_model.meshes.push_back(std::move(new_submesh));
        }

        imported_data.models.push_back(new_model);
    }

    return imported_data;
};

}  // namespace Presto
