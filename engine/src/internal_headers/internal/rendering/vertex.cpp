#include <algorithm>
#include <numeric>
#include <vector>

#include "internal/rendering/vertex.h"
#include "internal/rendering/vertex_types.h"

namespace Pr {

std::vector<PipelineAttribute> Vertex3D::getPipelineAttributes() {
    return {PipelineAttribute{.layout = 0,
                              .type = ShaderDataType::VEC3,
                              .name = DefaultAttributeName::POSITION,
                              .offset = offsetof(Vertex3D, vertexPosition)},
            PipelineAttribute{
                .layout = 1,
                .type = ShaderDataType::VEC3,
                .name = DefaultAttributeName::COLOUR,
                .offset = offsetof(Vertex3D, colour),
            },
            PipelineAttribute{
                .layout = 2,
                .type = ShaderDataType::VEC3,
                .name = DefaultAttributeName::NORMAL,
                .offset = offsetof(Vertex3D, normal),
            },
            PipelineAttribute{
                .layout = 3,
                .type = ShaderDataType::VEC2,
                .name = DefaultAttributeName::TEXCOORDS,
                .offset = offsetof(Vertex3D, tex_coords),
            }};
};

std::vector<PipelineAttribute> VertexUI::getPipelineAttributes() {
    return {
        PipelineAttribute{.layout = 0,
                          .type = ShaderDataType::VEC2,
                          .name = DefaultAttributeName::POSITION,
                          .offset = 0},
        PipelineAttribute{.layout = 1,
                          .type = ShaderDataType::VEC4,
                          .name = DefaultAttributeName::COLOUR,
                          .offset = 8},
        PipelineAttribute{.layout = 2,
                          .type = ShaderDataType::VEC2,
                          .name = DefaultAttributeName::TEXCOORDS,
                          .offset = 20},

    };
};

std::vector<PipelineAttribute> VertexDebug::getPipelineAttributes() {
    return {
        PipelineAttribute{.layout = 0,
                          .type = ShaderDataType::VEC3,
                          .name = DefaultAttributeName::POSITION,
                          .offset = 0},
        PipelineAttribute{.layout = 1,
                          .type = ShaderDataType::VEC4,
                          .name = DefaultAttributeName::COLOUR,
                          .offset = 12},

    };
};

bool AttributeSet::lock(bool calculateOffsets) {
    if (locked_) {
        PR_CORE_WARN(
            "Attempted to lock an attribute set that is already locked. "
            "Ignoring this request.");
        return false;
    }

    if (calculateOffsets) {
        Pr::size_t running_offset{0};

        for (auto& [key, value] : attributes_) {
            value.offset = running_offset;
            running_offset += value.size();
        }
    }

    locked_ = true;
    return true;
};

void AttributeSet::addAttribute(VertexAttribute attrib) {
    PR_ASSERT(!this->locked(),
              "An AttributeSet must be locked in order to add new attributes.");

    // Check that the new attribute hasn't already been added to the map
    PR_CORE_ASSERT(attributes_.find(attrib.index) == attributes_.end(),
                   std::format("Attempted to bind an attribute to the "
                               "same index twice (index: {})",
                               attrib.index));

    attributes_[attrib.index] = std::move(attrib);
};

const VertexAttribute* AttributeSet::getAttribute(
    const Pr::string& name) const {
    auto found{std::ranges::find_if(attributes_, [name](const auto& pair) {
        return pair.second.name == name;
    })};

    if (found == attributes_.end()) {
        return nullptr;
    }

    return &(found->second);
}

const auto& AttributeSet::getAttributes() const { return attributes_; };

std::size_t AttributeSet::size() { return attributes_.size(); }

Pr::size_t AttributeSet::bytesRequired() const {
    PR_ASSERT(this->locked(),
              "Unable to obtain stride on an unlocked AttributeSet.");

    return std::accumulate(attributes_.begin(), attributes_.end(), 0,
                           [](Pr::size_t sum, const auto& pair) -> Pr::size_t {
                               return sum + pair.second.size();
                           });
}

constexpr Pr::size_t SizeOfType(UniformVariableType type) noexcept {
#define SWITCH_CASE(type) \
    case type:            \
        return sizeof(UniformVariableTypeTraits<type>::GPUType);

    switch (type) {
        SWITCH_CASE(UniformVariableType::INT);
        SWITCH_CASE(UniformVariableType::UINT);
        SWITCH_CASE(UniformVariableType::FLOAT);
        SWITCH_CASE(UniformVariableType::VEC2);
        SWITCH_CASE(UniformVariableType::VEC3);
        SWITCH_CASE(UniformVariableType::VEC4);
        SWITCH_CASE(UniformVariableType::MAT4);
        SWITCH_CASE(UniformVariableType::TEXTURE);
        default: {
            return 0;
        }
    }
#undef SWITCH_CASE
};

BaseAttributeTypeDetails getShaderTypeDetails(ShaderDataType type) {
    using namespace Pr;

#define SWITCH_CASE(type) \
    case type:            \
        return AttributeTypeDetails<ShaderImportTypeOf<(type)>>();
    switch (type) {
        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::VEC3);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::MAT3);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::MAT4);
        SWITCH_CASE(ShaderDataType::DMAT4);
        break;
    };

#undef SWITCH_CASE

    return {};
}

Pr::size_t VertexAttribute::size() const noexcept {
    return SizeOfShaderType(type);
};

}  // namespace Pr

constexpr Pr::size_t Pr::SizeOfShaderType(ShaderDataType type) {
    using namespace Pr;
#define SWITCH_CASE(type)                                         \
    case type:                                                    \
        return sizeof(ShaderDataTypeTraits<(type)>::ImportType) * \
               ShaderDataTypeTraits<(type)>::subtype_count;

    switch (type) {
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);

        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC3);

        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::MAT3);

        SWITCH_CASE(ShaderDataType::MAT4);

        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::DMAT4);

        default: {
            PR_ERROR("No size available.");
            return 0;
        }
    }
#undef SWITCH_CASE
}
