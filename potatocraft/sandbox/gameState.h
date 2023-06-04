#pragma once

#include "potatoengine/engineAPI.h"

namespace potatocraft {

class GameState : public potatoengine::State {
   public:
    GameState(const std::shared_ptr<potatoengine::AssetsManager> &assetsManager);
    virtual ~GameState() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate(potatoengine::Time dt) override;
    virtual void onImGuiRender() override;
    virtual void onEvent(potatoengine::Event& event) override;

    static std::unique_ptr<State> Create(const std::shared_ptr<potatoengine::AssetsManager> &assetsManager);

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
