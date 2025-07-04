module;
#include "presto/platform.h"

export module presto.objects.components.render;

import std;

import presto.core.platform;
import presto.objects.component;
import presto.assets.types;

export namespace Pr {
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

    RenderComponent& addModel(const Pr::Ptr<Pr::ModelAsset>& model);

    [[nodiscard]] std::vector<Pr::Ptr<Pr::ModelAsset>>& getModels() const;

   private:
    void onEnterScene() override;

    struct Impl;
    Allocated<Impl> impl_;
};

struct PRESTO_API QuadSubcomponent : public Subcomponent<RenderComponent> {
    float width;
    float height;

    TransformData transform;
    Pr::Ptr<Pr::MaterialInstance> material;
};

struct PRESTO_API ModelSubcomponent : public Subcomponent<RenderComponent> {
    std::vector<MeshDraw> draws;

    [[nodiscard]] Pr::size_t meshCount() const { return draws.size(); }
};

}  // namespace Pr
