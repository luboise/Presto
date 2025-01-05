#include "Presto/Module.h"

#include "Presto/Resources/MeshResource.h"

using nlohmann::json;

namespace Presto {
    class Mesh;

    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void Init();

        MeshResource& LoadMeshFromDisk(const AssetPath&);

        // Deleted functions
        ResourceManager() = delete;
        virtual ~ResourceManager() = delete;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        // Gets the type of name from the MeshResource struct
        std::map<decltype(MeshResource::name), MeshResource*> meshResources_;
    };
}  // namespace Presto
