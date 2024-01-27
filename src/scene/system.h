#pragma once

#include <cstdint>
#include <entt/entt.hpp>

#include "events/event.h"
#include "core/time.h"

namespace potatoengine::systems {

class System {
  public:
    System(int32_t priority = 0) : m_priority(priority) {}
    virtual ~System() = default;

    int32_t getPriority() const { return m_priority; }

    virtual void init(entt::registry& registry){};
    virtual void update(entt::registry& registry, const Time& ts){};

  protected:
    int32_t m_priority = 0;
};

struct SystemComparator {
    bool operator()(const std::pair<std::string, std::unique_ptr<System>>& lhs,
                    const std::pair<std::string, std::unique_ptr<System>>& rhs) const {
      return lhs.second->getPriority() < rhs.second->getPriority();
    }
};

}