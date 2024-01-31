#include "core/statesManager.h"
#include "utils/mapJsonSerializer.h"

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
  if (m_idx > 0) {
    // TODO rethink this
    m_states[m_idx - 1]->onDetach();
  }
  m_states.emplace(m_states.begin() + m_idx++, std::move(s));
  m_states[m_idx - 1]->onAttach();
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
    if (m_idx > 0) {
      // TODO rethink this
      m_states[m_idx - 1]->onAttach();
    }
  }
  m_dirty = true;
}

void StatesManager::pushLayer(std::unique_ptr<Layer>&& l) {
  ENGINE_ASSERT(m_idx > 0, "No states to push layer to");
  m_states[m_idx - 1]->getLayersManager()->pushLayer(std::move(l));
  m_dirty = true;
}

void StatesManager::pushOverlay(std::unique_ptr<Layer>&& o, bool enabled) {
  ENGINE_ASSERT(m_idx > 0, "No states to push overlay to");
  m_states[m_idx - 1]->getLayersManager()->pushOverlay(std::move(o), enabled);
  m_dirty = true;
}

void StatesManager::enableOverlay(std::string_view name) {
  ENGINE_ASSERT(m_idx > 0, "No states to enable overlay in");
  m_states[m_idx - 1]->getLayersManager()->enableOverlay(name);
  m_dirty = true;
}

void StatesManager::disableOverlay(std::string_view name) {
  ENGINE_ASSERT(m_idx > 0, "No states to disable overlay in");
  m_states[m_idx - 1]->getLayersManager()->disableOverlay(name);
  m_dirty = true;
}

const std::map<std::string, std::string, NumericComparator>&
StatesManager::getMetrics() {
  if (not m_dirty) {
    return m_metrics;
  }

  m_metrics.clear();
  for (const auto& state : m_states | std::views::take(m_idx)) {
    std::map<std::string, std::string, NumericComparator> layers;
    for (const auto& layer : state->getLayersManager()->getLayers()) {
      layers[layer->getName().data()] =
        layer->isEnabled() ? "enabled" : "disabled";
    }
    m_metrics[state->getName().data()] = MapToJson(layers);
  }
  m_dirty = false;

  return m_metrics;
}

std::unique_ptr<StatesManager> StatesManager::Create() {
  return std::make_unique<StatesManager>();
}
}
