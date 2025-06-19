module presto.internal.debugging:constants;

import presto.core.constants;

namespace Presto {

using CheckedComponentBits = uint32_t;
using CheckedComponentBit = CheckedComponentBits;

constexpr CheckedComponentBit TRANSFORM_BIT = AS_BIT(0);
constexpr CheckedComponentBit RENDER_BIT = AS_BIT(1);
constexpr CheckedComponentBit CAMERA_BIT = AS_BIT(2);

constexpr auto PR_MASTERPIECE_EXTENSION = ".masterpiece";

constexpr auto PR_SCENE_EXTENSION = ".prsc";
constexpr auto PR_MESH_LIB_EXTENSION = ".prmdb";

}  // namespace Presto
