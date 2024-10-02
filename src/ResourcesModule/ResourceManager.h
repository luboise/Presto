#include "Presto/Module.h"

namespace Presto {
    class Mesh;

    class PRESTO_API ResourceManager : public Module {
        MODULE_FUNCTIONS();

       public:
        ResourceManager();
        ~ResourceManager();

        static Mesh* LoadMesh(const std::string&);

        static std::vector<char> readFile(const std::string& filename);
        static void Init();
    };
}  // namespace Presto
