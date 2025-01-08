#pragma once

#include "Presto/Objects/Component.h"
#include "Presto/Rendering/RenderData.h"

namespace Presto {
    class PRESTO_API Renderable : public Component {
        friend class RenderingManager;

       public:
        [[nodiscard]] virtual RenderData getRenderData() const = 0;

       private:
    };

}  // namespace Presto
