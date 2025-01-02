#include "PrestoCore/Module.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace Presto {
    class Mesh;

    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void Init();

        ResourceManager() = delete;
        virtual ~ResourceManager() = delete;

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        static Mesh* LoadMesh(const std::string&);

        static std::string ReadFile(const std::string& filename);

        static json GetJSON(const std::string& text);
    };
}  // namespace Presto
