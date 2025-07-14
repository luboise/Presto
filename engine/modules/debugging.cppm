export module presto.debugging;

import presto.core;
import presto.aliases.object;

import presto.math;

export namespace Pr {

/**
 * @brief  Draws a line on the next frame. This has no memory, so debug draws
 * which need to be repeated must be called once per frame.
 */
void DrawLine(Pr::vec3 from, Pr::vec3 to, Pr::vec4 colour);

/**
 * @brief  Draws a cube on the next frame.
 */

void Draw(const Pr::Cube& cube, Pr::vec4 colour = Colour.WHITE);

/**
 * @brief  Draw the debug shapes of a camera
 */
void Draw(Camera& camera);

/**
 * @brief  Enable or disable the debug drawing of the main camera
 */
void DebugMainCamera(bool = true);

}  // namespace Pr
