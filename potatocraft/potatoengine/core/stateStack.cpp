#include "potatoengine/core/stateStack.h"

namespace potatoengine {

StateStack::~StateStack() {
    for (auto& state : m_states) {
        state->onDetach();
    }
}

void StateStack::pushState(std::unique_ptr<State> state) {
    m_states.emplace(m_states.begin() + m_idx++, std::move(state));
}

void StateStack::pushOverlay(std::unique_ptr<State> overlay) {
    m_states.emplace_back(std::move(overlay));
}

void StateStack::popState(std::string_view name) {
    auto it = std::ranges::find_if(m_states | std::views::take(m_idx), [&](const auto& state) {
        return state->getName() == name;
    });
    if (it not_eq m_states.begin() + m_idx) {
        (*it)->onDetach();
        m_states.erase(it);
        --m_idx;
    }
}

void StateStack::popOverlay(std::string_view name) {
    m_states.erase(std::remove_if(m_states.begin(), m_states.end(), [&](const auto& state) {
        return state->getName() == name;
    }), m_states.end());
}

std::unique_ptr<StateStack> StateStack::Create() {
    return std::make_unique<StateStack>();
}

}
