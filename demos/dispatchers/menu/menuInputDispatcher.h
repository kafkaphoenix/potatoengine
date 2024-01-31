#pragma once

#include "engineAPI.h"
#include "states/gameState.h"

namespace demos::dispatchers {

inline bool onMenuKeyPressed(engine::events::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.repeating())
    return false;

  if (e.getKeyCode() == engine::Key::Enter) {
    engine::Application::Get().getStatesManager()->pushState(
      states::GameState::Create());
    return true;
  }

  return false;
}

inline void menuInputDispatcher(engine::events::Event& e) {
  engine::events::EventDispatcher dispatcher(e);

  dispatcher.dispatch<engine::events::KeyPressedEvent>(
    BIND_STATIC_EVENT(onMenuKeyPressed));
}
}