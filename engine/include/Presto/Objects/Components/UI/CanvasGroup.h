#pragma once

#include "CanvasItem.h"

namespace Presto {
class PRESTO_API CanvasGroup {
    friend class CanvasComponent;

   public:
    ~CanvasGroup() = default;

    void addItem(CanvasItem item) { items_.emplace_back(std::move(item)); };
    void addItem(CanvasItem&& item) { items_.emplace_back(std::move(item)); };

    [[nodiscard]] const auto& items() const { return this->items_; }

    CanvasGroup(const CanvasGroup&) = default;
    CanvasGroup(CanvasGroup&&) = default;

    CanvasGroup& operator=(const CanvasGroup&) = delete;
    CanvasGroup& operator=(CanvasGroup&&) = delete;

   private:
    CanvasGroup() = default;

    std::vector<CanvasItem> items_;
};
}  // namespace Presto
