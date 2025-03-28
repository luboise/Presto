#include <algorithm>

#include "Presto/Rendering/BoundingBox.h"

using std::abs;

namespace Presto {
void BoundingBox::merge(const BoundingBox& other) {
    this->x_min = std::min(x_min, other.x_min);
    this->y_min = std::min(y_min, other.y_min);
    this->z_min = std::min(z_min, other.z_min);

    this->x_max = std::min(x_max, other.x_max);
    this->y_max = std::min(y_max, other.y_max);
    this->z_max = std::min(z_max, other.z_max);
}

double BoundingBox::getNormalisingFactor() const {
    double new_val{0};

    double max{x_min};

    new_val = abs(x_max);
    max = std::max(new_val, max);

    new_val = abs(y_min);
    max = std::max(new_val, max);

    new_val = abs(y_max);
    max = std::max(new_val, max);

    new_val = abs(z_min);
    max = std::max(new_val, max);

    new_val = abs(z_max);
    max = std::max(new_val, max);

    if (max == 0) {
        return 0;
    }

    return 0.5 / max;
}
}  // namespace Presto
