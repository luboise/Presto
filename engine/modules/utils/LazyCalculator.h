#pragma once

namespace Presto {
class LazyCalculator {
    using DirtyBitType = bool;

   public:
    [[nodiscard]] bool isDirty() const { return dirty_; }

   protected:
    void setDirty(DirtyBitType dirty = true) { dirty_ = dirty; }

   private:
    DirtyBitType dirty_;
};
}  // namespace Presto
