#pragma once

#include "Presto/Rendering/UniformTypes.h"

namespace Presto {

struct UniformLayout {
    std::vector<UniformBinding> bindings;
    std::vector<UniformBlock> blocks;
};

struct MaterialProperty {
    PR_STRING_ID name;
    UniformVariableType type;

    Presto::size_t binding;
    Presto::uint8_t offset;

    static bool compatible(const MaterialProperty&, const MaterialProperty&);
};

struct MaterialBlockProperty;

struct PipelineStructure;

/*
struct UniformStructure {
    std::vector<MaterialProperty> properties;
    ErasedBytes data;

    [[nodiscard]] const MaterialProperty* getProperty(
        const PR_STRING_ID& name) const;

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
