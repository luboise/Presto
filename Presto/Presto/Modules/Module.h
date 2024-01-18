#pragma once
// #include "prpch.h"

namespace Presto {
    class PRESTO_API Module {
       public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        inline bool IsInitialised() const { return this->_initialised; };

       protected:
        bool _initialised;
    };
}  // namespace Presto