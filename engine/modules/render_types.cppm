export module presto.render_types;

import presto.assets.image;
import presto.core.types;

#pragma once

export namespace Pr {

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
