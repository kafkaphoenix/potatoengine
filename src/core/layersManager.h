#pragma once

#include "core/layer.h"
#include "pch.h"

namespace potatoengine {

class LayersManager {
  public:
    ~LayersManager();

    void pushLayer(std::unique_ptr<Layer>&& l);
    void pushOverlay(std::unique_ptr<Layer>&& o, bool enabled);
    void enableOverlay(std::string_view name);
    void disableOverlay(std::string_view name);
    const std::vector<std::unique_ptr<Layer>>& getLayers() const {
      return m_layers;
    }
    void clear();

    auto begin() { return m_layers.begin(); }
    auto end() { return m_layers.end(); }
    auto rbegin() { return m_layers.rbegin(); }
    auto rend() { return m_layers.rend(); }

    auto begin() const { return m_layers.begin(); }
    auto end() const { return m_layers.end(); }
    auto rbegin() const { return m_layers.rbegin(); }
    auto rend() const { return m_layers.rend(); }

    static std::unique_ptr<LayersManager> Create();

  private:
    std::vector<std::unique_ptr<Layer>> m_layers;
    uint32_t m_idx{};
};

}