#pragma once

#include "events/event.h"

namespace potatoengine {

class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

    float getX() const noexcept { return m_mouseX; }
    float getY() const noexcept { return m_mouseY; }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_mouseX{}, m_mouseY{};
};

class MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

    float getX() const noexcept { return m_mouseX; }
    float getY() const noexcept { return m_mouseY; }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_mouseX{}, m_mouseY{};
};

class MouseButtonEvent : public Event {
  public:
    uint32_t GetMouseButton() const noexcept { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

  protected:
    MouseButtonEvent(uint32_t b) : m_button(b) {}

    uint32_t m_button{};
};

class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(uint32_t b) : MouseButtonEvent(b) {}

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(uint32_t b) : MouseButtonEvent(b) {}

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}