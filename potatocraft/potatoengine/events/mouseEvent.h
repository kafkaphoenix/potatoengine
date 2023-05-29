#pragma once

#include "potatoengine/events/event.h"

namespace potatoengine {

class MouseMovedEvent : public Event {
   public:
    MouseMovedEvent(const float x, const float y)
        : m_mouseX(x), m_mouseY(y) {}

    float getX() const { return m_mouseX; }
    float getY() const { return m_mouseY; }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

   private:
    float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event {
   public:
    MouseScrolledEvent(const float x, const float y)
        : m_mouseX(x), m_mouseY(y) {}

    float getX() const { return m_mouseX; }
    float getY() const { return m_mouseY; }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

   private:
    float m_mouseX, m_mouseY;
};

class MouseButtonEvent : public Event {
   public:
    uint32_t GetMouseButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

   protected:
    MouseButtonEvent(const uint32_t button)
        : m_button(button) {}

    uint32_t m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
   public:
    MouseButtonPressedEvent(const uint32_t button)
        : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
   public:
    MouseButtonReleasedEvent(const uint32_t button)
        : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}