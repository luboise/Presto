#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API ResourceManager : public Module {
       public:
        ResourceManager();
        ~ResourceManager();

        static std::vector<char> readFile(const std::string& filename);

        void Init();
        void Shutdown();
    };
}  // namespace Presto