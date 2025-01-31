#pragma once

#include "Presto/Components/UI/CanvasItem.h"

namespace Presto {
    class PRESTO_API CanvasGroup {
        friend class CanvasComponent;

       public:
        void addItem(CanvasItem item);
        ~CanvasGroup() = default;

       private:
        CanvasGroup() = default;
    };
}  // namespace Presto
