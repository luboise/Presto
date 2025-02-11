#include "Presto/Assets/Image.h"

namespace Presto {

const std::vector<Presto::size_t> default_texture_bytes{
    0,   0,   0,   255,  // black
    255, 255, 255, 255,  // white
    255, 255, 255, 255,  // white
    0,   0,   0,   255   // white
};

static const Presto::Image DEFAULT_TEXTURE{
    .width = 2,
    .height = 2,
    .bytes{default_texture_bytes.begin(), default_texture_bytes.end()}};

}  // namespace Presto
