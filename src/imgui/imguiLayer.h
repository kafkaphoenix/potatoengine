#pragma once

#include "core/layer.h"

namespace potatoengine {

class ImGuiLayer : public Layer {
  public:
    ImGuiLayer();

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onImguiUpdate() override;
    static void begin();
    static void end();
};
}