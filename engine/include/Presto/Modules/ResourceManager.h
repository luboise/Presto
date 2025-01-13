#pragma once

#include "Module.h"

#include "Presto/Core/Types.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    enum class ResourceType { JSON, RAW };

    class PRESTO_API ResourceManager : public Module<ResourceManager> {
       public:
        static void Init();
        void Update() override {}

        using mesh_key_t = decltype(MeshResource::name);
        MeshResource& loadMeshFromDisk(const AssetPath& path,
                                       const mesh_key_t& customName);

        [[nodiscard]] MeshResource* getMesh(mesh_key_t) const;

        // Deleted functions
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

       private:
        ResourceManager() = default;

        // Gets the type of name from the MeshResource struct
        std::map<mesh_key_t, std::unique_ptr<MeshResource>> meshResources_;
    };
}  // namespace Presto
