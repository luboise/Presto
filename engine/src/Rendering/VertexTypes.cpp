#include "Presto/Rendering/VertexTypes.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {

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
                          .type = ShaderDataType::VEC3,
                          .name = DefaultAttributeName::COLOUR,
                          .offset = 8},
        PipelineAttribute{.layout = 2,
                          .type = ShaderDataType::VEC3,
                          .name = DefaultAttributeName::TEXCOORDS,
                          .offset = 20},

    };
};

}  // namespace Presto
