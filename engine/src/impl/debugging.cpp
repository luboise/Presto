module presto.debugging;

import presto.internal.managers.debug_manager;

void Pr::Draw(const Pr::Cube& cube, Pr::vec4 colour) {
    using namespace Pr;

    static auto& dm{DebugManager::get()};

    const auto vertices{cube.vertices()};
    // Front face
    dm.drawLine(vertices[0], vertices[1], {.colour = colour});
    dm.drawLine(vertices[1], vertices[2], {.colour = colour});
    dm.drawLine(vertices[2], vertices[3], {.colour = colour});
    dm.drawLine(vertices[3], vertices[0], {.colour = colour});
    // Back face
    dm.drawLine(vertices[4], vertices[5], {.colour = colour});
    dm.drawLine(vertices[5], vertices[6], {.colour = colour});
    dm.drawLine(vertices[6], vertices[7], {.colour = colour});
    dm.drawLine(vertices[7], vertices[4], {.colour = colour});
    // Connecting edges
    dm.drawLine(vertices[0], vertices[4], {.colour = colour});
    dm.drawLine(vertices[1], vertices[5], {.colour = colour});
    dm.drawLine(vertices[2], vertices[6], {.colour = colour});
    dm.drawLine(vertices[3], vertices[7], {.colour = colour});
};

void Pr::DrawLine(Pr::vec3 from, Pr::vec3 to, Pr::vec4 colour) {
    using namespace Pr;

    auto& dm{DebugManager::get()};

    dm.drawLine(from, to, {.colour{colour}});
}

void Pr::Draw(Camera& camera) {
    using namespace Pr;

    auto& dm{DebugManager::get()};

    Rectangle far_rect{camera.farRectangle()};
    Rectangle near_rect{camera.nearRectangle()};

    dm.drawRect(far_rect, {.colour{Colour.BLUE}});
    dm.drawRect(near_rect, {.colour{Colour.RED}});

    vec3 pos{camera.position()};

    DebugDrawProps line_props{.colour{Colour.WHITE}};
    dm.drawLine(pos, far_rect.top_left, line_props);
    dm.drawLine(pos, far_rect.top_right, line_props);
    dm.drawLine(pos, far_rect.at(1, 1), line_props);
    dm.drawLine(pos, far_rect.bottom_left, line_props);
};

void Pr::DebugMainCamera(bool enabled) {
    using namespace Pr;

    DebugManager::get().setDrawMainCamera(enabled);
}
