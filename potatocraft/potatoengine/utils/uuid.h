#pragma once
#include "potatoengine/pch.h"

namespace potatoengine {

static std::random_device rd;
static std::mt19937_64 gen(rd());
static std::uniform_int_distribution<uint64_t> dis;

class UUID {
   public:
    UUID() : m_uuid(dis(gen)) {}

    operator uint64_t() const { return m_uuid; }
   private:
    uint64_t m_uuid;
};
}