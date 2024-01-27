#include "core/stateMachine.h"

namespace potatoengine {

StateMachine::~StateMachine() {
  ENGINE_WARN("Deleting state machine");
  for (auto& s : m_states) {
    s->onDetach();
  }
  m_states.clear();
  m_metrics.clear();
  m_dirty = false;
}

void StateMachine::pushState(std::unique_ptr<State>&& s) {
  m_states.emplace(m_states.begin() + m_idx++, std::move(s));
  m_dirty = true;
}

void StateMachine::pushOverlay(std::unique_ptr<State>&& o) {
  m_states.emplace_back(std::move(o));
  m_dirty = true;
}

void StateMachine::popState(std::string_view name) {
  auto it = std::ranges::find_if(
    m_states | std::views::take(m_idx),
    [&](const auto& state) { return state->getName() == name; });
  if (it not_eq m_states.begin() + m_idx) {
    (*it)->onDetach();
    m_states.erase(it);
    --m_idx;
  }
  m_dirty = true;
}

void StateMachine::popOverlay(std::string_view name) {
  m_states.erase(
    std::remove_if(m_states.begin(), m_states.end(),
                   [&](const auto& state) { return state->getName() == name; }),
    m_states.end());
  m_dirty = true;
}

std::map<std::string, std::string, NumericComparator>&
StateMachine::getMetrics() {
  if (not m_dirty) {
    return m_metrics;
  }

  m_metrics.clear();
  for (int i = 0; i < m_idx; ++i) {
    m_metrics["state_" + std::to_string(i)] = m_states[i]->getName();
  }
  m_metrics["state_idx"] = std::to_string(m_idx);
  m_dirty = false;

  return m_metrics;
}

std::unique_ptr<StateMachine> StateMachine::Create() {
  return std::make_unique<StateMachine>();
}
}
