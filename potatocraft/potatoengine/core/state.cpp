#include "potatoengine/core/state.h"

#include "potatoengine/pch.h"

namespace potatoengine {

State::State(const std::string& name) : m_name(name) {}

std::unique_ptr<State> State::Create() noexcept {
    return std::make_unique<State>();
}
}