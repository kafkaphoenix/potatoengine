#pragma once

#include "engineAPI.h"
namespace demos {

class GameState : public engine::State {
  public:
    GameState(const std::unique_ptr<engine::Settings>& settings);

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onUpdate(engine::Time ts) override final;
    virtual void onImguiUpdate() override final;
    virtual void onEvent(engine::Event& e) override final;

    static std::unique_ptr<State> Create(const std::unique_ptr<engine::Settings>& settings);
};
}
