#pragma once

#include "potatoengine/core/state.h"

namespace potatoengine {

class StateStack {
   public:
    StateStack() = default;
    ~StateStack();

    void pushState(State* state);
    void pushOverlay(State* overlay);
    void popState(const std::string& name);
    void popOverlay(const std::string& name);

    auto begin() { return m_states.begin(); }
    auto end() { return m_states.end(); }
    auto rbegin() { return m_states.rbegin(); }
    auto rend() { return m_states.rend(); }

    auto begin() const { return m_states.begin(); }
    auto end() const { return m_states.end(); }
    auto rbegin() const { return m_states.rbegin(); }
    auto rend() const { return m_states.rend(); }

   private:
    std::vector<State*> m_states;
    uint32_t m_stateInsertIndex{};
};

}