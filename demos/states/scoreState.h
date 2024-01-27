#pragma once

#include "engineAPI.h"

namespace demos::states {

class ScoreState : public engine::State {
  public:
    ScoreState();

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onUpdate(const engine::Time& ts) override final;
    virtual void onImguiUpdate() override final {}
    virtual void onEvent(engine::events::Event& e) override final;

    static std::unique_ptr<State> Create();
};
}
