module;
#include "presto/events.h"

export module presto.internal.events;

import presto.core.event;
import presto.core.types;

export namespace Presto {

/**
 * WindowResizeEvent
 * - Created when window is resized.
 * - Has a width and height parameter (the new width and height of the
 * window)
 */
class WindowResizeEvent : public Event {
   public:
    WindowResizeEvent(unsigned new_width, unsigned new_height)
        : width(new_width), height(new_height) {}

    [[nodiscard]] unsigned GetWidth() const { return width; }
    [[nodiscard]] unsigned GetHeight() const { return height; }

    [[nodiscard]] std::string toString() const override {
        return std::format("WindowResizeEvent: {}, {}", width, height);
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)

   private:
    unsigned width, height;
};

struct WindowData;

class FramebufferResizedEvent : public Event {
   public:
    explicit FramebufferResizedEvent(const WindowData* data);

    [[nodiscard]] unsigned width() const { return width_; }
    [[nodiscard]] unsigned height() const { return height_; }

    [[nodiscard]] const WindowData* windowData() const { return windowData_; }

    [[nodiscard]] VisualExtents extents() const {
        return {
            .width = static_cast<uint16_t>(width()),
            .height = static_cast<uint16_t>(height()),
        };
    }

    [[nodiscard]] std::string toString() const override {
        return std::format("FramebufferResizedEvent: {}, {}", width_, height_);
    }

    EVENT_CLASS_TYPE(FramebufferResized)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)

   private:
    const WindowData* windowData_;

    unsigned width_, height_;
};

class WindowCloseEvent : public Event {
   public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class AppTickEvent : public Event {
   public:
    AppTickEvent() = default;

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class AppUpdateEvent : public Event {
   public:
    AppUpdateEvent() = default;

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class AppRenderEvent : public Event {
   public:
    AppRenderEvent() = default;

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class ObjectEvent : public Event {
   public:
    [[nodiscard]] void* getPtr() const { return this->_objectPtr; }

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryObject)

   protected:
    // Constructor
    template <typename T>
    explicit ObjectEvent(T* objectPtr) : _objectPtr((void*)(objectPtr)) {}
    void* _objectPtr;
};

class ObjectCreatedEvent : public ObjectEvent {
   public:
    EVENT_CLASS_TYPE(ObjectCreated)

    template <typename T>
    explicit ObjectCreatedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

    [[nodiscard]] std::string toString() const override {
        return std::format("Entity created at address 0x{}", _objectPtr);
    }
};

class ObjectDestroyedEvent : public ObjectEvent {
   public:
    EVENT_CLASS_TYPE(ObjectDestroyed)

    template <typename T>
    explicit ObjectDestroyedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

    [[nodiscard]] std::string toString() const override {
        return std::format("Entity destroyed at address 0x{}", _objectPtr);
    }
};

}  // namespace Presto
