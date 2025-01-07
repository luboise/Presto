#pragma once

namespace Presto {
    class Time {
        friend class Application;

       public:
        static double deltaMilliseconds();
        static double deltaSeconds();

        static double totalSecondsSinceStart();

       private:
        static void update();
        static void init();

        struct Impl;
        static std::unique_ptr<Impl> impl_;
    };

}  // namespace Presto
