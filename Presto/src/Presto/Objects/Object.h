#pragma once
#include "prpch.h"

namespace Presto {
    class PRESTO_API vec3 {
       public:
        vec3(float) private : float _x;
        float _y;
        float _z;
    };

    class PRESTO_API Object {
       private:
        Vec2 _position
    };

}  // namespace Presto