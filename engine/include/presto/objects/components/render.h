#include <vector>
#include "presto/core/platform.h"

#include "presto/assets/asset.h"
#include "presto/handles/asset_handles.h"
#include "presto/math/transform_data.h"
#include "presto/objects/component.h"

namespace Pr {
struct QuadSubcomponent;

class PRESTO_API RenderComponent final : public Component {
    // friend class RenderingManager;
    friend class EntityManager;
    friend class EntityManagerImpl;

    // TODO: Make the render trigger when the entity enters the scene so
    // that this friend declaration isn't necessary
    friend class RenderingManager;

   public:
    RenderComponent();
    ~RenderComponent() override;

    RenderComponent& addQuad(const QuadSubcomponent& quad);
    RenderComponent& addQuad(QuadSubcomponent&& quad);
    [[nodiscard]] std::vector<QuadSubcomponent>& getQuads() const;

    RenderComponent& addModel(const ModelPtr& model);

    [[nodiscard]] std::vector<ModelPtr>& getModels() const;

   private:
    void onEnterScene() override;

    struct Impl;
    Allocated<Impl> impl_;
};

struct PRESTO_API QuadSubcomponent : public Subcomponent<RenderComponent> {
    float width;
    float height;

    TransformData transform;
    MaterialPtr material;
};

struct PRESTO_API ModelSubcomponent : public Subcomponent<RenderComponent> {
    std::vector<MeshDraw> draws;

    [[nodiscard]] Pr::size_t meshCount() const { return draws.size(); }
};

}  // namespace Pr
