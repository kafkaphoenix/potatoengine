#pragma once

#include "engineAPI.h"

namespace demos {

struct CState {
    enum class State {
      INVALID,
      MENU,
      STARTING,
      READY,
      RUNNING,
      PAUSED,
      STOPPED,
      GAMEOVER
    };

    State state{State::INVALID};

    CState() = default;
    explicit CState(State state) : state(state) {}

    std::string_view getState() const {
      switch (state) {
      case State::MENU: return "MENU";
      case State::STARTING:
        return "STARTING"; // TODO this should be overlay loading
      case State::READY:
        return "READY"; // TODO we dont need this we the overlay
      case State::RUNNING: return "RUNNING";
      case State::PAUSED: return "PAUSED";
      case State::STOPPED: return "STOPPED";
      case State::GAMEOVER: return "GAMEOVER";
      case State::INVALID: return "INVALID";
      default: return "UNKNOWN";
      }
    }

    void print() const { ENGINE_BACKTRACE("\t\tstate: {0}", getState()); }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["state"] = getState();

      return info;
    }
};
}