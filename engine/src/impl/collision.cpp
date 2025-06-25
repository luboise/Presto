module presto.collision;

Pr::CollisionResult Pr::Colliding(const Ray& ray, const Plane& plane) {
    using namespace Pr;
    CollisionResult res{.colliding = true};

    // For xz plane only
    // auto t_value{(ray.D.y - plane.N.y) / ray.D.y};

    auto t_value{-(glm::dot(ray.origin - plane.point, plane.normal) /
                   glm::dot(ray.direction, plane.normal))};

    if (!ray.contains(t_value)) {
        res.colliding = false;
        return res;
    };

    res.collision_point = ray.at(t_value);
    return res;
}
