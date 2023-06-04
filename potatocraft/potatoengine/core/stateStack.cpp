#include "potatoengine/core/stateStack.h"

namespace potatoengine {

StateStack::~StateStack() {
    for (auto& state : m_states) {
        state->onDetach();
    }
}

void StateStack::pushState(std::unique_ptr<State> state) {
    m_states.emplace(m_states.begin() + m_index++, std::move(state));
}

void StateStack::pushOverlay(std::unique_ptr<State> overlay) {
    m_states.emplace_back(std::move(overlay));
}

void StateStack::popState(const std::string& name) {
    auto it = std::find_if(m_states.begin(), m_states.begin() + m_index, [&](const std::unique_ptr<State>& state) { return state->getName() == name; });

    if (it != m_states.begin() + m_index) {
        (*it)->onDetach();
        m_states.erase(it);
        --m_index;
    }
}

void StateStack::popOverlay(const std::string& name) {
    auto it = std::find_if(m_states.rbegin(), m_states.rend(), [&](const std::unique_ptr<State>& state) { return state->getName() == name; });

    if (it != m_states.rend()) {
        (*it)->onDetach();
        m_states.erase(std::next(it).base());
    }
}

std::unique_ptr<StateStack> StateStack::Create() {
    return std::make_unique<StateStack>();
}

}
