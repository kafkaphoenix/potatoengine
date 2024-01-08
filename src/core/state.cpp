#include "core/state.h"

#include "pch.h"

namespace potatoengine {

State::State(std::string&& name) : m_name(std::move(name)) {}
}