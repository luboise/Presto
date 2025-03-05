#pragma once

#include "CanvasItem.h"

namespace Presto {
class PRESTO_API CanvasGroup {
    friend class CanvasComponent;

   public:
    ~CanvasGroup() = default;

    void addItem(CanvasItem item) { items_.push_back(std::move(item)); };
    [[nodiscard]] const auto& items() const { return this->items_; }

   private:
    CanvasGroup() = default;

    std::vector<CanvasItem> items_;
};
}  // namespace Presto
