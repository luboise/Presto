#pragma once

namespace Presto {
    class PRESTO_API Entity {
       public:
        Entity();
        virtual ~Entity();

        uint32_t getId() const;

       private:
        uint32_t _id;

        static uint32_t getNextAvailableId(void);
        static uint32_t _currentId;
    };

    typedef Entity* entity_t;
}  // namespace Presto