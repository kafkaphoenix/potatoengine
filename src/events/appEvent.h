#pragma once

#include "events/event.h"

namespace potatoengine::events {

class AppTickEvent : public Event {
  public:
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event {
  public:
    AppUpdateEvent(std::string&& dispatcher_target) : m_dispatcherTarget(dispatcher_target) {}

    std::string_view getDispatcherTarget() const noexcept { return m_dispatcherTarget; }

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
  
  private:
    std::string m_dispatcherTarget;
};

class AppRenderEvent : public Event {
  public:
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}