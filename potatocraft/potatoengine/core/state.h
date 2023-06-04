#pragma once

#include "potatoengine/core/time.h"
#include "potatoengine/events/event.h"

namespace potatoengine {

class State {
   public:
    State(const std::string& name = "TemplateState");
    virtual ~State() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(Time) {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event&) {}

    const std::string& getName() const { return m_name; }

    static std::unique_ptr<State> Create();

   protected:
    std::string m_name;
};

}