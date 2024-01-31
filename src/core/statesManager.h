#pragma once

#include "core/state.h"
#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

class StatesManager {
  public:
    ~StatesManager();

    void pushState(std::unique_ptr<State>&& s);
    void popState(std::string_view name);
    void pushLayer(std::unique_ptr<Layer>&& l);
    void pushOverlay(std::unique_ptr<Layer>&& o, bool enabled);
    void enableOverlay(std::string_view name);
    void disableOverlay(std::string_view name);
    const std::unique_ptr<State>& getCurrentState() {
      ENGINE_ASSERT(m_idx > 0, "No states to get current state from");
      return m_states[m_idx - 1];
    }
    uint32_t getStateIdx() { return m_idx; }

    const std::map<std::string, std::string, NumericComparator>& getMetrics();

    static std::unique_ptr<StatesManager> Create();

  private:
    std::vector<std::unique_ptr<State>> m_states;
    uint32_t m_idx{};
    std::map<std::string, std::string, NumericComparator> m_metrics;
    bool m_dirty{};
};

}