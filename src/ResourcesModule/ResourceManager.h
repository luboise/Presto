#include "Presto/Module.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace Presto {
    class Mesh;

    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module {
        MODULE_FUNCTIONS();
        void Init();

       public:
        ResourceManager();
        ~ResourceManager();

        static Mesh* LoadMesh(const std::string&);

        // static std::vector<char> ReadFile(const std::string& filename);

        template <ResourceType T>
        static auto ReadFile(const std::string& filename);
    };
}  // namespace Presto
