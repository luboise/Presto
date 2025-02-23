#include "Presto/Rendering/RendererFactory.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

// DO NOT MOVE! Need to import gl.h before glew.h

#include "Presto/Runtime/GLFWAppWindow.h"

using Presto::Renderer, Presto::Allocated;

Allocated<Renderer> Presto::CreateRenderer(RENDER_LIBRARY lib,
                                           GLFWAppWindow* window) {}
