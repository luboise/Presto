#include "GLFWAppWindow.h"

namespace Presto {
    class EditorOverlay {
       public:
        static void Init(GLFWAppWindow*);

        static void OnLoopStart();
        static void OnLoopEnd();

       private:
        static GLFWAppWindow* _window;
    };
}  // namespace Presto
