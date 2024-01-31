#pragma once

#include "engineAPI.h"

namespace demos::layers {

class LevelCompletedOverlay : public engine::Layer {
  public:
    LevelCompletedOverlay();

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onEvent(engine::events::Event& e) override final;

    static std::unique_ptr<engine::Layer> Create();

  private:
    bool onKeyPressed(engine::events::KeyPressedEvent& e);
};
}
