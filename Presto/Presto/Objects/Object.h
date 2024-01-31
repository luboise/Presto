#pragma once

namespace Presto {
    class PRESTO_API Object {
       public:
        Object();
        virtual ~Object();

        uint32_t getId() const;

       private:
        uint32_t _id;

        static uint32_t getNextAvailableId(void);
        static uint32_t _currentId;
    };
}  // namespace Presto