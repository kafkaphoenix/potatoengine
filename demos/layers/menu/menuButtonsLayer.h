#pragma once

#include "engineAPI.h"

namespace demos::layers {

class MenuButtonsLayer : public engine::Layer {
  public:
    MenuButtonsLayer();

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onEvent(engine::events::Event& e) override final;

    static std::unique_ptr<engine::Layer> Create();
};
}
