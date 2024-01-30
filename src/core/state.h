#pragma once

#include "core/time.h"
#include "events/event.h"
#include "pch.h"

namespace potatoengine {

class State {
  public:
    State(std::string&& name = "TemplateState") : m_name(std::move(name)) {}
    virtual ~State() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(const Time&) {}
    virtual void onImguiUpdate() {}
    virtual void onEvent(events::Event&) {}

    std::string_view getName() const noexcept { return m_name; }

  protected:
    std::string m_name;
};

}