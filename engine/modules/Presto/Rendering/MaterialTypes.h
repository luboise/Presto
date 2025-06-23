#pragma once

#include "Presto/Rendering/UniformTypes.h"

export namespace Presto {

struct MaterialBlockProperty;

struct PipelineStructure;

/*
struct UniformStructure {
    std::vector<MaterialProperty> properties;
    ErasedBytes data;

    [[nodiscard]] const MaterialProperty* getProperty(
        const Presto::string& name) const;

    [[nodiscard]] bool writeableFrom(const UniformStructure&) const;

    void merge(const UniformStructure&);

    static UniformStructure from(const PipelineStructure&);

    void writeFrom(const MaterialProperty& other);

[[nodiscard]] bool compatibleWith(const MaterialProperty& other) const {
    return MaterialProperty::compatible(*this, other);
};

};
*/

}  // namespace Presto
