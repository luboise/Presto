#pragma once
// #include "prpch.h"

#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API RenderingModule : public Module {
       public:
        virtual PR_RESULT drawFrame() = 0;
    };
}  // namespace Presto