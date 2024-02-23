#include <Presto/Presto.h>

#include <glm/gtc/constants.hpp>

namespace Pr = Presto;

std::vector<Pr::VulkanVertex> makeHeart(uint32_t points) {
    // Symmetric shape, so need even number of points
    if (points % 2 == 1) points++;

    std::vector<Pr::VulkanVertex> heart(points);

    const float width = 1;
    const float height = width;

    const double denom = glm::two_pi<double>() / (points);
    double t = 0;
    for (auto i = 0; i < points; i++) {
        t = i * denom;

        float x = (width / 2.0f) * glm::pow(glm::sin(t), 3);
        // Normalised height
        float y = (13 * glm::cos(t) - 2 * glm::cos(3 * t) - glm::cos(4 * t) -
                   5 * glm::cos(2 * t));
        y += 2.5;
        y /= 15;
        // Scale to desired height
        y *= (height / 2);

        heart[i] = Pr::VulkanVertex{{x, -y, 0.0f}, glm::vec4(1, 0, 0, 1)};
    }

    return heart;
};

class DualityApp : public Presto::Application {
   public:
    DualityApp() {
        this->_heart = Pr::EntityManager::newEntity();

        Pr::VertexList vertices = makeHeart(200);
        Pr::Polygon2D* shape(vertices);

        this->_heart.addComponent();
    }

    ~DualityApp() {}

   private:
    Pr::Entity* _heart;
};

Presto::Application* Presto::CreateApplication() { return new DualityApp(); }