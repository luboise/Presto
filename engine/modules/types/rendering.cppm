export module presto.types.rendering;

import presto.types.core;
import std;

export namespace Pr {

struct VisualExtents {
    std::uint16_t width;
    std::uint16_t height;

    [[nodiscard]] double getAspectRatio() const {
        return (double)width / (double)height;
    }
};

enum class MeshDrawMode : Pr::uint8_t {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP
};

using Index = uint32_t;
using IndexList = std::vector<Index>;

using uniform_name_t = Pr::string;

using mesh_context_id_t = Pr::uint32_t;
using mesh_registration_id_t = Pr::uint32_t;
using pipeline_id_t = Pr::uint16_t;
using material_id_t = Pr::uint16_t;
using texture_id_t = Pr::uint16_t;

namespace DefaultAttributeName {

constexpr auto POSITION = "a_vertexPosition";

constexpr auto BASE_COLOUR = "u_baseColour";

constexpr auto COLOUR = "a_colour";
constexpr auto COLOR = COLOUR;

constexpr auto NORMAL = "a_normal";
constexpr auto TEXCOORDS = "a_texcoords";

bool IsDefaultAttributeName(const Pr::string& str);

}  // namespace DefaultAttributeName

namespace DefaultMaterialPropertyName {

constexpr const char* BASE_COLOUR = "u_baseColour";
// Alias for American english speakers
constexpr const char* BASE_COLOR = BASE_COLOUR;

constexpr const char* DIFFUSE_TEXTURE = "u_diffuseTexture";

constexpr const char* UI_POSITION = "u_position";
constexpr const char* UI_SIZE = "u_size";
constexpr const char* UI_OFFSET = "u_offset";

};  // namespace DefaultMaterialPropertyName

}  // namespace Pr
