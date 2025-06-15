#include "Presto/Core.h"

#include "Presto/Rendering/MaterialInstance.h"  // IWYU pragma: export
#include "Presto/Types/MaterialTypes.h"

namespace Presto {

MaterialPtr NewMaterial(MaterialType type, Presto::string name = "");
MaterialPtr FindMaterial(const Presto::string& name);

}  // namespace Presto
