export module presto.debugging;

import presto.core.types;

#pragma once

// #include "Presto/Aliases/ObjectAliases.h"
// #include "Presto/Core/Colours.h"

namespace Presto {
struct Cube;

/**
 * @brief  Draws a line on the next frame. This has no memory, so debug draws
 * which need to be repeated must be called once per frame.
 */
void DrawLine(Presto::vec3 from, Presto::vec3 to, Presto::vec4 colour);

/**
 * @brief  Draws a cube on the next frame.
 */

void Draw(const Presto::Cube& cube, Presto::vec4 colour = Colour.WHITE);

/**
 * @brief  Draw the debug shapes of a camera
 */
void Draw(Camera& camera);

/**
 * @brief  Enable or disable the debug drawing of the main camera
 */
void DebugMainCamera(bool = true);

}  // namespace Presto
