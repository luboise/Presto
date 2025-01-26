#pragma once

namespace Presto {
    class Material {
       public:
        virtual void bind() = 0;
        virtual void unbind() = 0;
    };
}  // namespace Presto
