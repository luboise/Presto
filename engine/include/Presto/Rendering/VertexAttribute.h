#pragma once

#include "AttributeTypes.h"

#include "Presto/Rendering/ShaderTypes.h"
#include "Presto/Types/CoreTypes.h"

#include <map>

namespace Presto {

using vertex_binding_t = std::uint16_t;

struct VertexAttribute {
    // AttributeTypeDetails typeDetails;
    ShaderDataType type;

    PR_STRING_ID name;
    vertex_binding_t index;
    Presto::size_t offset{0};

    [[nodiscard]] std::size_t size() const noexcept {
        return SizeOfShaderType(type);
    };
};

class AttributeSet {
   public:
    void addAttribute(VertexAttribute attrib);

    [[nodiscard]] const VertexAttribute* getAttribute(
        const PR_STRING_ID& name) const;

    [[nodiscard]] const auto& getAttributes() const;

    [[nodiscard]] std::size_t size();

    bool lock(bool calculateOffsets = false);

    [[nodiscard]] bool locked() const { return locked_; }

    [[nodiscard]] std::size_t bytesRequired() const;

    // Alias of bytesRequired
    [[nodiscard]] std::size_t stride() const { return bytesRequired(); }

   private:
    bool locked_{false};

    std::map<vertex_binding_t, VertexAttribute> attributes_;
};
}  // namespace Presto
