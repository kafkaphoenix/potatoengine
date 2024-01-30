#include "core/statesManager.h"

namespace potatoengine {

StatesManager::~StatesManager() {
  ENGINE_WARN("Deleting states manager");
  for (auto& s : m_states) {
    s->onDetach();
  }
  m_states.clear();
  m_metrics.clear();
  m_dirty = false;
}

void StatesManager::pushState(std::unique_ptr<State>&& s) {
  m_states.emplace(m_states.begin() + m_idx++, std::move(s));
  m_dirty = true;
}

void StatesManager::pushOverlay(std::unique_ptr<State>&& o) {
  m_states.emplace_back(std::move(o));
  m_dirty = true;
}

void StatesManager::popState(std::string_view name) {
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

void StatesManager::popOverlay(std::string_view name) {
  m_states.erase(
    std::remove_if(m_states.begin(), m_states.end(),
                   [&](const auto& state) { return state->getName() == name; }),
    m_states.end());
  m_dirty = true;
}

std::map<std::string, std::string, NumericComparator>&
StatesManager::getMetrics() {
  if (not m_dirty) {
    return m_metrics;
  }

  m_metrics.clear();
  for (uint32_t i = 0; i < m_idx; ++i) {
    m_metrics["state_" + std::to_string(i)] = m_states[i]->getName();
  }
  m_metrics["state_idx"] = std::to_string(m_idx);
  m_dirty = false;

  return m_metrics;
}

std::unique_ptr<StatesManager> StatesManager::Create() {
  return std::make_unique<StatesManager>();
}
}
