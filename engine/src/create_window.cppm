export module presto.internal.create_window;

import presto.runtime.window;

export namespace Pr {

// Window create function that must be implemented per platform
// Uses default props if unspecified
WindowPtr CreateWindow(const WindowProperties& props = WindowProperties());

}  // namespace Pr
