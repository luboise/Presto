#pragma once

namespace Pr {
template <typename HandleType>
struct Abstraction {
   public:
    explicit operator HandleType() const { return this->handle_; };
    HandleType handle() const {
        Pr::CoreAssert(
            handle_ != nullptr,
            "An abstracted type has a handle of nullptr. This shouldn't be "
            "possible, as its lifetime should be tied to the constructor "
            "and destructor as per RAII. Object: {}",
            fmt::ptr(this));

        return this->handle_;
    };

   protected:
    HandleType handle_;
};

}  // namespace Pr
