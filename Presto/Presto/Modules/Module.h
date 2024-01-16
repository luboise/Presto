#pragma once
// #include "prpch.h"

namespace Presto {
    class PRESTO_API Module {
       public:
        static enum MODULE_LAUNCH_CODE {
            SUCCESS = 0,
            FAILURE = 1,
        };

        virtual MODULE_LAUNCH_CODE start() = 0;
        virtual void stop() = 0;
    };
}  // namespace Presto