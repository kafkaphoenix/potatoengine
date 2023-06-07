#pragma once

#include "potatoengine/engineAPI.h"

namespace potatocraft {

class GameState : public potatoengine::State {
   public:
    GameState(const std::shared_ptr<potatoengine::AssetsManager> &am);
    virtual ~GameState() = default;

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onUpdate(potatoengine::Time dt) override final;
    virtual void onImGuiRender() override final;
    virtual void onEvent(potatoengine::Event& e) override final;

    static std::unique_ptr<State> Create(const std::shared_ptr<potatoengine::AssetsManager> &am);

   private:
    bool onKeyPressed(potatoengine::KeyPressedEvent& e);
    bool onKeyReleased(potatoengine::KeyReleasedEvent& e);
    void computeShaders();
    void loadCubes();

    bool m_debugging = false;
    bool m_wireframe = false;

    std::shared_ptr<potatoengine::Program> m_basic;
    std::shared_ptr<potatoengine::VAO> m_vao;
    potatoengine::CameraController m_cameraController;
    potatoengine::Scene m_scene;
    std::shared_ptr<potatoengine::AssetsManager> m_assetsManager;
};
}
