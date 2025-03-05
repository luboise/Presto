#pragma once

#include "Presto/Rendering/RenderTypes.h"

namespace Presto::DefaultMeshes {

const MeshData Quad =  // Default quad
    {.pipeline_id = PR_PIPELINE_DEFAULT_UI,
     .draw_mode = MeshDrawMode::TRIANGLES,
     .vertices = std::vector{VertexUI{.vertexPosition{-0.5, 0.5},
                                      .colour{1, 1, 1},
                                      .texcoords{-1, 1}},
                             VertexUI{.vertexPosition{0.5, 0.5},
                                      .colour{1, 1, 1},
                                      .texcoords{1, 1}},
                             VertexUI{.vertexPosition{-0.5, -0.5},
                                      .colour{1, 1, 1},
                                      .texcoords{-1, -1}},
                             VertexUI{.vertexPosition{0.5, -0.5},
                                      .colour{1, 1, 1},
                                      .texcoords{-1, 1}}},
     .indices{0, 1, 2, 1, 2, 3}};

}
