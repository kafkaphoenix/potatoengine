#pragma once

#include "engineAPI.h"

namespace demos::layers {

class GameLayer : public engine::Layer {
  public:
    GameLayer();

    virtual void onDetach() override final;
    virtual void onUpdate(const engine::Time& ts) override final;
    virtual void onEvent(engine::events::Event& e) override final;

    static std::unique_ptr<engine::Layer> Create();

  private:
    bool onKeyPressed(engine::events::KeyPressedEvent& e);
};
}
