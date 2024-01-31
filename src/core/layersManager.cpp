#include "core/layersManager.h"

namespace potatoengine {

LayersManager::~LayersManager() {
  ENGINE_WARN("Deleting layers manager");
  for (auto& l : m_layers) {
    if (l->isEnabled()) {
      l->onDetach();
    }
  }
  m_layers.clear();
}

void LayersManager::pushLayer(std::unique_ptr<Layer>&& l) {
  l->onAttach();
  l->setEnabled(true);
  m_layers.emplace(m_layers.begin() + m_idx++, std::move(l));
}

void LayersManager::pushOverlay(std::unique_ptr<Layer>&& o, bool enabled) {
  if (enabled) {
    o->onAttach();
    o->setEnabled(true);
  }
  m_layers.emplace_back(std::move(o));
}

void LayersManager::enableOverlay(std::string_view name) {
  auto it = std::ranges::find_if(
    m_layers | std::views::drop(m_idx),
    [&](const auto& layer) { return layer->getName() == name; });
  if (it not_eq m_layers.end()) {
    if (not(*it)->isEnabled()) {
      (*it)->onAttach();
      (*it)->setEnabled(true);
    } else {
      ENGINE_WARN("Overlay {} is already enabled", name);
    }
  }
}

void LayersManager::disableOverlay(std::string_view name) {
  auto it = std::ranges::find_if(
    m_layers | std::views::drop(m_idx),
    [&](const auto& layer) { return layer->getName() == name; });
  if (it not_eq m_layers.end()) {
    if ((*it)->isEnabled()) {
      (*it)->onDetach();
      (*it)->setEnabled(false);
    } else {
      ENGINE_WARN("Overlay {} is already disabled", name);
    }
  }
}

void LayersManager::clear() {
  for (auto& l : m_layers) {
    if (l->isEnabled()) {
      l->onDetach();
    }
  }
  m_layers.clear();
  m_idx = 0;
}

std::unique_ptr<LayersManager> LayersManager::Create() {
  return std::make_unique<LayersManager>();
}
}
