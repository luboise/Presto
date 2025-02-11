#pragma once

namespace Presto {

enum class ShaderDataType {
    INT,
    UINT,
    FLOAT,
    DOUBLE,

    VEC2,
    DVEC2,

    VEC3,
    DVEC3,

    VEC4,
    DVEC4,

    MAT4,
    DMAT4
};
using attribute_size_t = Presto::size_t;

enum class ShaderDataSubType { INT, UINT, FLOAT, DOUBLE };

constexpr Presto::size_t shaderDataTypeSize(
    ShaderDataSubType subType) noexcept {
    switch (subType) {
        case ShaderDataSubType::INT:
            return sizeof(std::int32_t);

        case ShaderDataSubType::UINT:
            return sizeof(std::uint32_t);

        case ShaderDataSubType::FLOAT:
            return sizeof(Presto::float32_t);

        case ShaderDataSubType::DOUBLE:
            return sizeof(Presto::float64_t);
    }
};

struct AttributeTypeDetails {
    ShaderDataSubType sub_type;
    Presto::size_t sub_type_size;
    Presto::size_t sub_type_count;

    [[nodiscard]] Presto::size_t size() const {
        return sub_type_size * sub_type_count;
    }
};

constexpr AttributeTypeDetails attributeDetails(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::INT:
            return {.sub_type = ShaderDataSubType::INT,
                    .sub_type_size = shaderDataTypeSize(ShaderDataSubType::INT),
                    .sub_type_count = 1};
        case ShaderDataType::UINT:
            return {.sub_type = ShaderDataSubType::UINT,
                    .sub_type_size = shaderDataTypeSize(ShaderDataSubType::INT),
                    .sub_type_count = 1};
        case ShaderDataType::FLOAT:
            return {
                .sub_type = ShaderDataSubType::FLOAT,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::FLOAT),
                .sub_type_count = 1};

        case ShaderDataType::VEC2:
            return {
                .sub_type = ShaderDataSubType::FLOAT,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::FLOAT),
                .sub_type_count = 2};
        case ShaderDataType::VEC3:
            return {
                .sub_type = ShaderDataSubType::FLOAT,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::FLOAT),
                .sub_type_count = 3};
        case ShaderDataType::VEC4:
            return {
                .sub_type = ShaderDataSubType::FLOAT,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::FLOAT),
                .sub_type_count = 4};
        case ShaderDataType::MAT4:
            return {
                .sub_type = ShaderDataSubType::FLOAT,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::FLOAT),
                .sub_type_count = 16};
        case ShaderDataType::DOUBLE:
            return {
                .sub_type = ShaderDataSubType::DOUBLE,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::DOUBLE),
                .sub_type_count = 1};
        case ShaderDataType::DVEC2:
            return {
                .sub_type = ShaderDataSubType::DOUBLE,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::DOUBLE),
                .sub_type_count = 2};
        case ShaderDataType::DVEC3:
            return {
                .sub_type = ShaderDataSubType::DOUBLE,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::DOUBLE),
                .sub_type_count = 3};
        case ShaderDataType::DVEC4:
            return {
                .sub_type = ShaderDataSubType::DOUBLE,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::DOUBLE),
                .sub_type_count = 4};
        case ShaderDataType::DMAT4:
            return {
                .sub_type = ShaderDataSubType::DOUBLE,
                .sub_type_size = shaderDataTypeSize(ShaderDataSubType::DOUBLE),
                .sub_type_count = 16};
    }
}

}  // namespace Presto
