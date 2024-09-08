#include "Presto/Module.h"

namespace Presto {
    class PRESTO_API ResourceManager : public Module {
        MODULE_FUNCTIONS();

       public:
        ResourceManager();
        ~ResourceManager();

        static std::vector<char> readFile(const std::string& filename);
        static void Init();
    };
}  // namespace Presto
