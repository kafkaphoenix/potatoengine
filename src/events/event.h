#pragma once

#include "pch.h"

namespace potatoengine::events {

enum class EventType {
  None = 0,
  AppTick,
  AppUpdate,
  AppRender,
  WindowClose,
  WindowResize,
  WindowRestored,
  WindowMinimized,
  WindowMaximized,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  // Each category is a bit flag
  None = 0,
  EventCategoryApplication = 1,
  EventCategoryWindow = 2,
  EventCategoryInput = 4,
  EventCategoryKeyboard = 8,
  EventCategoryMouse = 16,
  EventCategoryMouseButton = 32,
};

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType GetStaticType() { return EventType::type; }                 \
  virtual EventType getEventType() const override { return GetStaticType(); }  \
  virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                         \
  virtual int getCategoryFlags() const override { return category; }

class Event {
  public:
    virtual ~Event() = default;
    bool isHandled{}; // cannnot be returned by a method

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategoryFlags() const = 0;

    bool isInCategory(EventCategory category) const {
      return getCategoryFlags() & category;
    }
};

class EventDispatcher {
  public:
    EventDispatcher(Event& e) : m_event(e) {}

    template <typename Type, typename Func> bool dispatch(const Func& func) {
      if (m_event.getEventType() == Type::GetStaticType()) {
        m_event.isHandled |= func(static_cast<Type&>(m_event));
        return true;
      }
      return false;
    }

  private:
    Event& m_event;
};

}