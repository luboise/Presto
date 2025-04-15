#pragma once

#include "Presto/Objects/Component.h"

namespace Presto {

struct ModelSubcomponent;
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
    [[nodiscard]] const std::vector<QuadSubcomponent>& getQuads() const;

    RenderComponent& addModel(const ModelSubcomponent& model);
    RenderComponent& addModel(ModelSubcomponent&& model);
    [[nodiscard]] const std::vector<ModelSubcomponent>& getModels() const;

   private:
    void onEnterScene() override;

    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
