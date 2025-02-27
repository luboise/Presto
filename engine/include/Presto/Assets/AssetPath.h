#pragma once

#include "Presto/Types/CoreTypes.h"

namespace Presto {

class AssetPath {
   public:
    AssetPath(const char*);  // Ignore warning, the conversion is intended

    explicit AssetPath(FilePath path);

    [[nodiscard]] Presto::string basename() const;

    [[nodiscard]] Presto::string fileExtension() const;

    [[nodiscard]] const FilePath& path() const;

    operator const FilePath&() const;
    explicit operator FilePath() const;

    [[nodiscard]] Presto::string string() const;
    explicit operator Presto::string() const;

    [[nodiscard]] bool valid() const;

   private:
    FilePath path_;
};

}  // namespace Presto
