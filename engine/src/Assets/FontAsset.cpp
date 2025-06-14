#include "Presto/Assets/FontAsset.h"

namespace Presto {

FontAsset::FontAsset(PR_STRING_ID name) : Asset(std::move(name)) {}

// TODO: Implement
bool FontAsset::load() { return true; };

}  // namespace Presto
