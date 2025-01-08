#pragma once

#include "Module.h"

#include "Presto/Resources/MeshResource.h"

namespace Presto {
    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void Init();
        void Update() override {}

        [[nodiscard]] MeshResource& LoadMeshFromDisk(const AssetPath&);

        // Deleted functions
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        ResourceManager() = default;

        // Gets the type of name from the MeshResource struct
        std::map<decltype(MeshResource::name), std::unique_ptr<MeshResource>>
            meshResources_;
    };
}  // namespace Presto
