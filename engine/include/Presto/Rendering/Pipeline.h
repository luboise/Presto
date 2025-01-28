#pragma once

namespace Presto {
    class Pipeline {
       public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setProperty(std::string property, const void* value) = 0;
    };
}  // namespace Presto
