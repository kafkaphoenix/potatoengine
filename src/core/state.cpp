#include "core/state.h"

#include "pch.h"

namespace potatoengine {

State::State(std::string&& name) : m_name(std::move(name)) {}

std::unique_ptr<State> State::Create() { return std::make_unique<State>(); }
}