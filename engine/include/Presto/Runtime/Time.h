#pragma once

namespace Presto {
    class Time {
        friend class Application;

       public:
        using Seconds = double;
        using Milliseconds = double;

        static Milliseconds deltaMilliseconds();
        static Seconds deltaSeconds();

        static Seconds totalSecondsSinceStart();

       private:
        static void update();
        static void init();

        struct Impl;
        static std::unique_ptr<Impl> impl_;
    };

}  // namespace Presto
