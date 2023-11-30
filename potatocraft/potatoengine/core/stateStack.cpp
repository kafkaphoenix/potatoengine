#include "potatoengine/core/stateStack.h"

namespace potatoengine {

StateStack::~StateStack() {
    ENGINE_WARN("Deleting StateStack");
    for (auto& s : m_states) {
        s->onDetach();
    }
}

void StateStack::pushState(std::unique_ptr<State>&& s) {
    m_states.emplace(m_states.begin() + m_idx++, std::move(s));
}

void StateStack::pushOverlay(std::unique_ptr<State>&& o) {
    m_states.emplace_back(std::move(o));
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

std::unique_ptr<StateStack> StateStack::Create() noexcept {
    return std::make_unique<StateStack>();
}

}
