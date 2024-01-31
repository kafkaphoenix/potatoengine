#pragma once

#include "engineAPI.h"

namespace demos::layers {

class MenuBackgroundLayer : public engine::Layer {
  public:
    MenuBackgroundLayer();

    virtual void onAttach() override final;
    virtual void onDetach() override final;

    static std::unique_ptr<engine::Layer> Create();
};
}
