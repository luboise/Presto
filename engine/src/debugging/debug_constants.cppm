module presto.internal.debugging:debug_constants;

import presto.core;

export namespace Pr {

using CheckedComponentBits = Pr::uint32_t;
using CheckedComponentBit = CheckedComponentBits;

constexpr CheckedComponentBit TRANSFORM_BIT = AS_BIT(0);
constexpr CheckedComponentBit RENDER_BIT = AS_BIT(1);
constexpr CheckedComponentBit CAMERA_BIT = AS_BIT(2);

constexpr auto PR_MASTERPIECE_EXTENSION = ".masterpiece";

constexpr auto PR_SCENE_EXTENSION = ".prsc";
constexpr auto PR_MESH_LIB_EXTENSION = ".prmdb";

}  // namespace Pr
