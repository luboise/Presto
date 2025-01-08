#pragma once

namespace Presto {
    template <typename HandleType>
    struct Abstraction {
        explicit operator HandleType() const { return this->_handle; };
        HandleType handle() const {
            PR_CORE_ASSERT(
                _handle != nullptr,
                "An abstracted type has a handle of nullptr. This shouldn't be "
                "possible, as its lifetime should be tied to the constructor "
                "and destructor as per RAII. Object: {}",
                fmt::ptr(this));

            return this->_handle;
        };

       protected:
        HandleType _handle;
    };

}  // namespace Presto
