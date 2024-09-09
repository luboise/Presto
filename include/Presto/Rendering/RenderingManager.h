#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Module.h"
#include "Presto/Rendering/Mesh.h"
#include "Presto/Rendering/Renderer.h"

#include "Presto/Rendering/types/RenderLayer.h"

#include "GLFW/glfw3.h"
#include "Presto/Tools/Allocator.h"

class RenderableProps;

namespace Presto {
    typedef uint32_t layer_id_t;

    class PRESTO_API RenderingManager : public Module {
        MODULE_FUNCTIONS();

       public:
        static void F_INIT(Renderer::RENDER_LIBRARY library,
                           GLFWwindow* windowPtr);

        static layer_id_t AddLayer(size_t pos = -1);
        static void RemoveLayer(layer_id_t id);

        static void AddRenderable(layer_id_t layer_index, Renderable*);
        static void RemoveRenderable(Renderable*);

        static void SetCamera(Camera& newCam);

        static Mesh* NewMesh(const VertexList&, const IndexList&);
        static RenderableProps* NewRenderableProps();
        static Renderable* NewRenderable(PrestoRenderableConstructorArgs);

       private:
        RenderingManager();

        inline static Renderer* _renderer;
        inline static GLFWwindow* _window;

        inline static std::vector<RenderLayer> _renderLayers;

        inline static Allocator<Mesh> _meshes;
        inline static Allocator<RenderableProps> _renderProps;
        inline static Allocator<Renderable> _renderables;

        // inline static std::map<id_t, Mesh*> _meshes;
        // inline static std::map<id_t, RenderableProps*> _renderProps;
        // inline static std::map<id_t, Renderable*> _renderables;

        inline static bool hasLayer(layer_id_t index);
        inline static RenderLayer& getLayer(layer_id_t id);
    };
}  // namespace Presto