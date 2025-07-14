module presto.internal.managers.debug_manager;

import presto.internal.managers.rendering_manager;

import presto.internal.debugging;

import presto.core.constants;
import presto.types;

import presto.internal.rendering.types;

import std;

namespace Pr {

constexpr Pr::size_t MAX_VERTEX_COUNT{100};
constexpr Pr::size_t MAX_INDEX_COUNT{MAX_VERTEX_COUNT * 2};

struct DebugManager::Impl {
    bool draw_main_camera{false};

    Allocated<MeshRegistrationData> draw_data{nullptr};

    Pr::size_t line_count{0};
    Pr::size_t vertex_count{0};

    Pr::size_t vb_offset{0};
    Pr::size_t ib_offset{0};
};

DebugManager::DebugManager(Pr::Window* windowPtr,
                           std::function<void()> exitCallback) {
    DebugUI::initialise(windowPtr, std::move(exitCallback));

    impl_ = std::make_unique<Impl>();
    impl_->draw_data = RenderingManager::get().allocateMeshRegistration(
        PR_PIPELINE_DEBUG_3D, sizeof(VertexDebug), MAX_VERTEX_COUNT,
        MAX_INDEX_COUNT);

    impl_->draw_data->draw_mode = MeshDrawMode::LINES;
};

void DebugManager::drawLine(vec3 from, vec3 to, DebugDrawProps props) {
    std::array<VertexDebug, 2> vertices = {
        VertexDebug{.vertexPosition{from}, .colour{props.colour}},
        VertexDebug{.vertexPosition{to}, .colour{props.colour}}};

    Pr::size_t vertices_size{sizeof(vertices)};

    ByteArray bytes(vertices_size);
    std::memcpy(bytes.data(), vertices.data(), vertices_size);

    impl_->draw_data->vertices->write(bytes, impl_->vb_offset);
    impl_->vb_offset += sizeof(vertices);

    std::array<Index, 2> indices = {
        static_cast<Index>(impl_->vertex_count),
        static_cast<Index>(impl_->vertex_count + 1)};

    Pr::size_t indices_size{sizeof(indices)};

    bytes.resize(indices_size);
    std::memcpy(bytes.data(), indices.data(), indices_size);
    impl_->draw_data->indices->write(bytes, impl_->ib_offset);

    impl_->ib_offset += indices_size;

    impl_->line_count += 1;
    impl_->vertex_count += 2;
};

void DebugManager::drawRect(const Rectangle& rect, DebugDrawProps props) {
    std::array<VertexDebug, 4> vertices = {
        VertexDebug{.vertexPosition{rect.top_left}, .colour{props.colour}},
        VertexDebug{.vertexPosition{rect.top_right}, .colour{props.colour}},
        VertexDebug{.vertexPosition{rect.at(1, 1)}, .colour{props.colour}},
        VertexDebug{.vertexPosition{rect.bottom_left}, .colour{props.colour}},
    };

    Pr::size_t vertices_size{sizeof(vertices)};

    ByteArray bytes(vertices_size);
    std::memcpy(bytes.data(), vertices.data(), vertices_size);

    impl_->draw_data->vertices->write(bytes, impl_->vb_offset);
    impl_->vb_offset += sizeof(vertices);

    std::array<Index, 8> indices = {0, 1, 1, 2, 2, 3, 3, 0};
    for (auto& val : indices) {
        val += impl_->vertex_count;
    }
    impl_->line_count += 4;
    impl_->vertex_count += 4;

    Pr::size_t indices_size{sizeof(indices)};

    bytes.resize(indices_size);
    std::memcpy(bytes.data(), indices.data(), indices_size);
    impl_->draw_data->indices->write(bytes, impl_->ib_offset);

    impl_->ib_offset += indices_size;
};

void DebugManager::update() {
    DebugUI::draw();
    DebugUI::render();
}

DebugManager::~DebugManager() = default;

void DebugManager::drawAll() {
    auto& rm{RenderingManager::get()};

    rm.usePipeline(PR_PIPELINE_DEBUG_3D);

    if (impl_->draw_main_camera) {
        Draw(*rm.getMainCamera()->getComponent<CameraComponent>());
    }

    impl_->draw_data->index_draw_count = impl_->line_count * 2;

    rm.drawFromAllocation(*impl_->draw_data);

    impl_->vb_offset = 0;
    impl_->ib_offset = 0;

    impl_->line_count = 0;
    impl_->vertex_count = 0;
}

void DebugManager::setDrawMainCamera(bool enabled) {
    impl_->draw_main_camera = enabled;
};

}  // namespace Pr
