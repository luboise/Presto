#pragma once

namespace Presto {
struct BoundingBox {
    double x_min{};
    double x_max{};

    double y_min{};
    double y_max{};

    double z_min{};
    double z_max{};

    void merge(const BoundingBox& other);

    /**
     *  @brief Calculates the co-efficient necessary in order to clamp a
     * bounding box to a 1x1x1 cube.
     * @warning At least one value of the bounding box must be
     * non-zero, otherwise 0 will be returned. This is likely to cause a
     * divide by zero somewhere.
     *  @return   The normalisation co-efficient.
     *
     * For example, if the bounding box has an x_min of -0.5, an x_max
     * of 1.5, a y_min of 0, a y_max of 2.0, a z_min of 0.1 and a z_max of
     * 0.3, the model will be scaled so that all values are between [-0.5,
     * 0.5].
     */
    double getNormalisingFactor();
};
}  // namespace Presto
