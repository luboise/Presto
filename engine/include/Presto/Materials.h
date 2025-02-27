#include "Presto/Types/MaterialTypes.h"

#include "Presto/Rendering/MaterialInstance.h"  // IWYU pragma: export

namespace Presto {

MaterialPtr NewMaterial(MaterialType type, Presto::string name = "");
MaterialPtr FindMaterial(Presto::string name);

}  // namespace Presto
