#pragma once
// #include "prpch.h"

namespace Presto {
    class PRESTO_API Module {
       public:
        virtual PR_RESULT Init() = 0;
        virtual void Shutdown() = 0;
    };
}  // namespace Presto