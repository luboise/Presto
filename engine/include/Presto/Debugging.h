#pragma once

#include "Presto/Aliases/ObjectAliases.h"

namespace Presto {
/**
 * @brief  Draws a line on the next frame. This has no memory, so debug draws
 * which need to be repeated must be called once per frame.
 */
void DrawLine(Presto::vec3 from, Presto::vec3 to, Presto::vec4 colour);

/**
 * @brief  Draw the debug shapes of a camera
 */
void Draw(Camera camera);

/**
 * @brief  Enable or disable the debug drawing of the main camera
 */
void DebugMainCamera(bool = true);

}  // namespace Presto
