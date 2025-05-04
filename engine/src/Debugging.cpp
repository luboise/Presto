#include <utility>

#include "Modules/DebugManager.h"
#include "Presto/Debugging.h"

#include "Modules/RenderingManager.h"

#include "Debugging/DebugUI.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Rendering/Buffer.h"

namespace Presto {

constexpr Presto::size_t MAX_VERTEX_COUNT{100};
constexpr Presto::size_t MAX_INDEX_COUNT{MAX_VERTEX_COUNT * 2};

void Presto::DrawLine(Presto::vec3 from, Presto::vec3 to, Presto::vec4 colour) {
    using namespace Presto;

    auto& dm{DebugManager::get()};

    dm.drawLine(from, to, colour);
}

struct DebugManager::Impl {
    Allocated<MeshRegistrationData> draw_data;
    Presto::size_t line_count{0};
};

DebugManager::DebugManager(Presto::Window* windowPtr,
                           std::function<void()> exitCallback) {
    DebugUI::initialise(windowPtr, std::move(exitCallback));

    impl_ = std::make_unique<Impl>();
    impl_->draw_data = RenderingManager::get().allocateForDrawing<VertexDebug>(
        PR_PIPELINE_DEBUG_3D, MAX_VERTEX_COUNT, MAX_INDEX_COUNT,
        MeshDrawMode::LINES);
};

void DebugManager::drawLine(vec3 from, vec3 to, vec4 colour) {
    std::array<VertexDebug, 2> vertices = {
        VertexDebug{.vertexPosition{from}, .colour{colour}},
        VertexDebug{.vertexPosition{from}, .colour{colour}}};

    ByteArray bytes(sizeof(vertices));
    std::memcpy(bytes.data(), vertices.data(), sizeof(vertices));

    Presto::size_t offset{impl_->line_count * sizeof(VertexDebug)};
    impl_->draw_data->vertices->write(bytes, offset);

    std::array<Index, 2> indices = {static_cast<Index>(impl_->line_count),
                                    static_cast<Index>(impl_->line_count)};

    offset = 2 * impl_->line_count++ * sizeof(Index);

    bytes.resize(sizeof(indices));
    std::memcpy(bytes.data(), indices.data(), sizeof(indices));
    impl_->draw_data->indices->write(bytes, offset);
};

void DebugManager::update() {
    DebugUI::render();

    for (const auto& draw : line) {
        draw line;
    }
}

}  // namespace Presto
