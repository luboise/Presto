#include "Presto/Rendering/Vertex.h"

namespace Presto::Meshes {

    using Presto::Vertex;

    constexpr std::pair<VertexList, IndexList> Cube(glm::vec3 position,
                                                    glm::float32 size,
                                                    glm::vec3 colour) {
        glm::float32 a = (size / 2);

        Vertex FRONT_BL = {glm::vec3{-a, a, a} + position, colour};
        Vertex FRONT_BR = {glm::vec3{a, a, a} + position, colour};
        Vertex BACK_BL = {glm::vec3{-a, a, -a} + position, colour};
        Vertex BACK_BR = {glm::vec3{a, a, -a} + position, colour};

        Vertex FRONT_TL = {glm::vec3{-a, -a, a} + position, colour};
        Vertex FRONT_TR = {glm::vec3{a, -a, a} + position, colour};
        Vertex BACK_TL = {glm::vec3{-a, -a, -a} + position, colour};
        Vertex BACK_TR = {glm::vec3{a, -a, -a} + position, colour};

        auto vertices = VertexList{FRONT_BL, FRONT_BR, BACK_BL, BACK_BR,

                                   FRONT_TL, FRONT_TR, BACK_TL, BACK_TR};

        auto indices = IndexList{
            // Back face
            2, 3, 6,  // Triangle 1: BACK_BL, BACK_BR, BACK_TR
            3, 7, 6,  // Triangle 2: BACK_BR, BACK_TR, BACK_TL

            // Left face
            0, 2, 4,  // Triangle 1: FRONT_BL, BACK_BL, BACK_TL
            2, 6, 4,  // Triangle 2: BACK_BL, BACK_TL, FRONT_TL

            // Right face
            1, 3, 5,  // Triangle 1: FRONT_BR, BACK_BR, BACK_TR
            3, 7, 5,  // Triangle 2: BACK_BR, BACK_TR, FRONT_TR

            // Top face
            0, 1, 2,  // Triangle 1: FRONT_BL, FRONT_BR, BACK_BL
            1, 3, 2,  // Triangle 2: FRONT_BR, BACK_BR, BACK_BL

            // Bottom face
            4, 5, 6,  // Triangle 1: FRONT_TL, FRONT_TR, BACK_TL
            5, 7,
            6,  // Triangle 2: FRONT_TR, BACK_TR, BACK_TL

            // Front face
            0, 1, 4,  // Triangle 1: FRONT_BL, FRONT_BR, FRONT_TR
            1, 5, 4,  // Triangle 2: FRONT_BR, FRONT_TR, FRONT_TL
        };

        return {vertices, indices};
    };
}  // namespace Presto::Meshes
