#pragma once

#include "core/time.h"
#include "events/event.h"

namespace potatoengine {

class State {
  public:
    State(std::string&& name = "TemplateState");
    virtual ~State() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(Time) {}
    virtual void onImguiUpdate() {}
    virtual void onEvent(Event&) {}

    std::string_view getName() const noexcept { return m_name; }

  protected:
    std::string m_name{};
};

}