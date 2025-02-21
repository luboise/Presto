#include "Presto/Assets/ImportTypes.h"

namespace Presto {

/*

void MaterialStructure::merge(const MaterialStructure& inStructure) {
std::ranges::for_each(this->properties, [&inStructure](
                                        MaterialProperty& property) {
const auto* in_property{inStructure.getProperty(property.name)};
PR_ASSERT(in_property != nullptr,
          "Incompatible MaterialStructure objects are being merged. "
          "Compatibility should be checked first using writeableFrom");

property = *in_property;
});
};

const MaterialProperty* MaterialStructure::getProperty(
const PR_STRING_ID& name) const {
if (const auto property{
    std::ranges::find_if(this->properties,
                         [name](const MaterialProperty& prop) -> bool {
                             return prop.name == name;
                         })};
property != this->properties.end()) {
return &*property;
}

return nullptr;
};

bool MaterialStructure::writeableFrom(
const MaterialStructure& inStructure) const {
return std::ranges::all_of(
this->properties, [&inStructure](const auto& out_property) -> bool {
    const MaterialProperty* in_property{
        inStructure.getProperty(out_property.name)};

    return in_property != nullptr &&
           in_property->compatibleWith(out_property);
});
};

void MaterialProperty::writeFrom(const MaterialProperty& other) {
this->data = other.data;

// TODO: Add type conversion here, so that vec3 values for instances can be
// used as vec2
};

bool MaterialProperty::compatible(const MaterialProperty& in,
                          const MaterialProperty& out) {
return in.type == out.type && in.name == out.name;

/*switch (out.type) {*/
/*        // In these cases, the in and out types must match*/
/*    case UniformVariableType::INT:*/
/*    case UniformVariableType::UINT:*/
/*    case UniformVariableType::FLOAT:*/
/*        return in.type == UniformVariableType::IMAGE;*/
/**/
/*    case UniformVariableType::VEC2:*/
/*    case UniformVariableType::VEC3:*/
/*    case UniformVariableType::MAT4:*/
/*    case UniformVariableType::IMAGE:*/
/*}*/
};

MaterialStructure MaterialStructure::from(const PipelineStructure& pipeline) {
        :SDFHLKSDJFHKSLDFJDL //
return {};
};

*/

}  // namespace Presto
