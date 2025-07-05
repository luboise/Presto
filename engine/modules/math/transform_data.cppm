export module presto.math:transform_data;

import :quaternion;

struct TransformData {
    Pr::vec3 position;
    Quaternion rotation;

    TransformData();
    explicit TransformData(Pr::vec3 position, Quaternion rotation = {});
    TransformData(Pr::vec3 position, Pr::vec3 rotation);

    Pr::vec3 scale{1, 1, 1};

    [[nodiscard]] Pr::mat4 asModelMat() const;
    [[nodiscard]] Pr::mat4 asViewMat() const;

    [[nodiscard]] Pr::vec3 forwards() const;
    [[nodiscard]] Pr::vec3 backwards() const;

    [[nodiscard]] Pr::vec3 leftwards() const;
    [[nodiscard]] Pr::vec3 rightwards() const;

    [[nodiscard]] Pr::vec3 upwards() const;
    [[nodiscard]] Pr::vec3 downwards() const;

    TransformData& addRotation(Pr::vec3);
    TransformData& addRotation(Pr::Quaternion);

    TransformData& addTranslation(Pr::vec3);
    TransformData& scaleBy(Pr::vec3);
};
