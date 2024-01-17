#include "_GraphicsRenderer.h"

namespace Presto {
    inline bool RenderingModule::IsInitialised() const {
        return this->_initialised;
    }
}  // namespace Presto