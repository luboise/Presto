export module presto.internal.rendering:vertex;

#include <algorithm>
#include <numeric>
#include <vector>

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
}  // namespace Pr
