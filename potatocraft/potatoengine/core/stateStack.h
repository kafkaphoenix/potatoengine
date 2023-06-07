#pragma once

#include "potatoengine/core/state.h"
#include "potatoengine/pch.h"

namespace potatoengine {

class StateStack {
   public:
    ~StateStack();

    void pushState(std::unique_ptr<State> state);
    void pushOverlay(std::unique_ptr<State> overlay);
    void popState(std::string_view name);
    void popOverlay(std::string_view name);

    auto begin() { return m_states.begin(); }
    auto end() { return m_states.end(); }
    auto rbegin() { return m_states.rbegin(); }
    auto rend() { return m_states.rend(); }

    auto begin() const { return m_states.begin(); }
    auto end() const { return m_states.end(); }
    auto rbegin() const { return m_states.rbegin(); }
    auto rend() const { return m_states.rend(); }

    static std::unique_ptr<StateStack> Create();
   private:
    std::vector<std::unique_ptr<State>> m_states;
    uint32_t m_idx;
};

}