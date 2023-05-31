#pragma once

#include "potatoengine/events/event.h"

namespace potatoengine {

class WindowResizeEvent : public Event {
   public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : m_width(width), m_height(height) {}

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

   private:
    uint32_t m_width, m_height;
};

class WindowCloseEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event {
   public:
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event {
   public:
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event {
   public:
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}