#pragma once
#include "pch.h"

namespace potatoengine {

static std::random_device rd;
static std::mt19937_64 gen(rd());
static std::uniform_int_distribution<uint32_t> dis;

class UUID {
  public:
    UUID() : m_uuid(dis(gen)) {}
    UUID(uint32_t uuid) : m_uuid(uuid) {}

    operator uint32_t() const noexcept { return m_uuid; }

  private:
    uint32_t m_uuid{};
};
}