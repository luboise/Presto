#pragma once

#include "Presto/Components/UI/CanvasItem.h"

namespace Presto {
    class PRESTO_API CanvasButton : public CanvasItem {
       public:
        explicit CanvasButton();

       private:
        CanvasPosition position;
    };
}  // namespace Presto
