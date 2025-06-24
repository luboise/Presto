#pragma once

// Defines that can be reused between child classes to get event types,
// consistent between classes
#define EVENT_CLASS_TYPE(type)                                             \
    static EventType getStaticType() { return EventType::type; }           \
    virtual EventType getType() const override { return getStaticType(); } \
    virtual const char* getName() const override { return #type; }

#define EVENT_USER_CLASS_TYPE(type)                                        \
    static EventType getStaticType() { return EventType::User; }           \
    virtual EventType getType() const override { return getStaticType(); } \
    virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                      \
    [[nodiscard]] virtual event_category_t getCategories() const override { \
        return category;                                                    \
    }
