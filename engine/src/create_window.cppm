export module presto.runtime.window.internal;

import presto.runtime.window;

export namespace Pr {

// Window create function that must be implemented per platform
// Uses default props if unspecified
WindowPtr Create(const WindowProperties& props = WindowProperties());

}  // namespace Pr
