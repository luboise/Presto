#pragma once

#define F_INIT Init
#define F_UPDATE Update
#define F_SHUTDOWN Shutdown

#define MODULE_FUNCTIONS()            \
   public:                            \
    static void F_UPDATE();           \
    static void F_SHUTDOWN();         \
                                      \
   private:                           \
    inline static bool _initialised = false; \
    static bool IsInitialised() { return _initialised; };

namespace Presto {
    class PRESTO_API Module {};
}  // namespace Presto