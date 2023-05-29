#include "potatoengine/core/stateStack.h"

namespace potatoengine {

StateStack::~StateStack() {
    for (auto state : m_states) {
        state->onDetach();
        delete state;
    }
}

void StateStack::pushState(State* state) {
    m_states.emplace(m_states.begin() + m_stateInsertIndex++, state);
}

void StateStack::pushOverlay(State* overlay) {
    m_states.emplace_back(overlay);
}

void StateStack::popState(const std::string& name) {
    auto it = std::find_if(m_states.begin(), m_states.begin() + m_stateInsertIndex, [&](const State* state) { return state->getName() == name; });

    if (it not_eq m_states.begin() + m_stateInsertIndex) {
        (*it)->onDetach();
        m_states.erase(it);
        --m_stateInsertIndex;
    }
}

void StateStack::popOverlay(const std::string& name) {
    auto it = std::find_if(m_states.rbegin(), m_states.rend(), [&](const State* state) { return state->getName() == name; });

    if (it not_eq m_states.rend()) {
        (*it)->onDetach();
        m_states.erase(std::next(it).base());
    }
}

}
