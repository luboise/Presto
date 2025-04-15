#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto::DefaultMeshes {

const MeshData Quad =  // Default quad
    {.pipeline_id = PR_PIPELINE_DEFAULT_UI,
     .draw_mode = MeshDrawMode::TRIANGLES,
     .vertices = std::vector{VertexUI{.vertexPosition{-0.5, 0.5},
                                      .colour{1, 1, 1},
                                      .tex_coords{-1, 1}},
                             VertexUI{.vertexPosition{0.5, 0.5},
                                      .colour{1, 1, 1},
                                      .tex_coords{1, 1}},
                             VertexUI{.vertexPosition{-0.5, -0.5},
                                      .colour{1, 1, 1},
                                      .tex_coords{-1, -1}},
                             VertexUI{.vertexPosition{0.5, -0.5},
                                      .colour{1, 1, 1},
                                      .tex_coords{-1, 1}}},
     .indices{0, 1, 2, 1, 2, 3}};

const MeshData ZeroedSquare = {
    .pipeline_id = PR_PIPELINE_DEFAULT_3D,
    .draw_mode = MeshDrawMode::TRIANGLES,
    .vertices =
        std::vector{
            Vertex3D{
                .vertexPosition{0, 0, 0}, .normal{0, 1, 0}, .tex_coords{0, 0}},
            Vertex3D{
                .vertexPosition{1, 0, 0}, .normal{0, 1, 0}, .tex_coords{1, 0}},
            Vertex3D{
                .vertexPosition{0, 1, 0}, .normal{0, 1, 0}, .tex_coords{0, 1}},
            Vertex3D{
                .vertexPosition{1, 1, 0}, .normal{0, 1, 0}, .tex_coords{1, 1}},

        },
    .indices{0, 1, 2, 1, 2, 3}};

}  // namespace Presto::DefaultMeshes
