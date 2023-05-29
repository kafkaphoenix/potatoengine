#pragma once

#include "potatoengine/engineAPI.h"

namespace potatocraft {

class GameState : public potatoengine::State {
   public:
    GameState();
    virtual ~GameState() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate(potatoengine::Time dt) override;
    virtual void onImGuiRender() override;
    virtual void onEvent(potatoengine::Event& event) override;

   private:
    bool onKeyPressed(potatoengine::KeyPressedEvent& e);
    bool onKeyReleased(potatoengine::KeyReleasedEvent& e);

    bool m_debugging = false;
    bool m_wireframe = false;

    std::shared_ptr<potatoengine::Program> m_shader;
    std::shared_ptr<potatoengine::VAO> m_vao;
    std::shared_ptr<potatoengine::Texture> m_texture;
    potatoengine::CameraController m_cameraController;
};
}
