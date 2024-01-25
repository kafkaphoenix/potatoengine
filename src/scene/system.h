#pragma once

#include <cstdint>
#include <entt/entt.hpp>

#include "core/time.h"

namespace potatoengine::systems {

class System {
  public:
    System(int32_t priority = 0) : m_priority(priority) {}
    virtual ~System() = default;

    int32_t getPriority() const { return m_priority; }

    virtual void init(entt::registry& r, const Time& ts) = 0;
    virtual void update(entt::registry& r, const Time& ts) = 0;
    virtual void shutdown(entt::registry& r, const Time& ts) = 0;

  protected:
    int32_t m_priority = 0;
};

struct SystemComparator {
    bool operator()(const std::unique_ptr<System>& lhs,
                    const std::unique_ptr<System>& rhs) const {
      return lhs->getPriority() < rhs->getPriority();
    }
};

}