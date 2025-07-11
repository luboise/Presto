export module presto.rendering.drawables;

import presto.types.core;
import std;

export namespace Pr {

struct Line {
    Pr::vec3 from{0, 0, 0};
    Pr::vec3 to{1, 1, 1};
    Pr::vec4 colour{1, 1, 1, 1};
};

}  // namespace Pr
