#pragma once

#include "core/time.h"
#include "events/event.h"
#include "pch.h"

namespace potatoengine {

class Layer {
  public:
    Layer(std::string&& name = "TemplateLayer") : m_name(std::move(name)) {}
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(const Time&) {}
    virtual void onImguiUpdate() {}
    virtual void onEvent(events::Event&) {}

    std::string_view getName() const { return m_name; }
    bool isEnabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }

  protected:
    std::string m_name;
    bool m_enabled{};
};

}