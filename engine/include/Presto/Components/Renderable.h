#pragma once

#include "Presto/Objects/Component.h"

namespace Presto {
    class PRESTO_API Renderable : public Component {
        friend class RenderingManager;

       public:
       private:
        Renderable() = default;
    };

}  // namespace Presto
