#pragma once

#include "core/keyCodes.h"
#include "events/event.h"

namespace potatoengine {

class KeyEvent : public Event {
  public:
    KeyCode getKeyCode() const noexcept { return m_keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

  protected:
    KeyEvent(const KeyCode keycode) : m_keyCode(keycode) {}

    KeyCode m_keyCode{};
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(const KeyCode keycode, bool repeating) : KeyEvent(keycode), m_repeating(repeating) {}

    bool repeating() const noexcept { return m_repeating; }

    EVENT_CLASS_TYPE(KeyPressed)

  private:
    bool m_repeating{};
};

class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
  public:
    KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyTyped)
};

}