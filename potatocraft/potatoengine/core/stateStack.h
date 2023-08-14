#pragma once

#include "potatoengine/core/state.h"
#include "potatoengine/pch.h"

namespace potatoengine {

class StateStack {
   public:
    ~StateStack();

    void pushState(std::unique_ptr<State> s);
    void pushOverlay(std::unique_ptr<State> o);
    void popState(std::string_view name);
    void popOverlay(std::string_view name);

    auto begin() noexcept { return m_states.begin(); }
    auto end() noexcept { return m_states.end(); }
    auto rbegin() noexcept { return m_states.rbegin(); }
    auto rend() noexcept { return m_states.rend(); }

    auto begin() const noexcept { return m_states.begin(); }
    auto end() const noexcept { return m_states.end(); }
    auto rbegin() const noexcept { return m_states.rbegin(); }
    auto rend() const noexcept { return m_states.rend(); }

    static std::unique_ptr<StateStack> Create() noexcept;

   private:
    std::vector<std::unique_ptr<State>> m_states{};
    uint32_t m_idx{};
};

}