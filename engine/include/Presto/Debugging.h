#pragma once

namespace Presto {
/**
 * @brief  Draws a line on the next frame. This has no memory, so debug draws
 * which need to be repeated must be called once per frame.
 */
void DrawLine(Presto::vec3 from, Presto::vec3 to, Presto::vec4 colour);

}  // namespace Presto
