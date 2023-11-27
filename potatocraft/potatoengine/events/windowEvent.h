#pragma once

#include "potatoengine/events/event.h"

namespace potatoengine {

class WindowCloseEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowResizeEvent : public Event {
   public:
    WindowResizeEvent(uint32_t w, uint32_t h)
        : m_width(w), m_height(h) {}

    uint32_t getWidth() const noexcept { return m_width; }
    uint32_t getHeight() const noexcept { return m_height; }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)

   private:
    uint32_t m_width{}, m_height{};
};

class WindowRestoredEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowRestored)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowMinimizedEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowMinimized)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowMaximizedEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowMaximized)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowFocusEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowFocus)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowLostFocusEvent : public Event {
   public:
    EVENT_CLASS_TYPE(WindowLostFocus)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowMovedEvent : public Event {
   public:
    WindowMovedEvent(int x, int y)
        : m_x(x), m_y(y) {}

    int getX() const noexcept { return m_x; }
    int getY() const noexcept { return m_y; }

    EVENT_CLASS_TYPE(WindowMoved)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)

   private:
    int m_x{}, m_y{};
};
}