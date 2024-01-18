#pragma once
// #include "prpch.h"

#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API RenderingModule : public Module {
       public:
        virtual PR_RESULT Init() = 0;
        virtual void Shutdown() = 0;

        bool IsInitialised() const;

       protected:
        bool _initialised = false;
    };
}  // namespace Presto