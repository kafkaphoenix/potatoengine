#pragma once

#include "potatoengine/engineAPI.h"

namespace potatocraft {

class GameState : public engine::State {
   public:
    GameState(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r);

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onUpdate(engine::Time ts) override final;
    virtual void onImGuiRender() override final;
    virtual void onEvent(engine::Event& e) override final;

    static std::unique_ptr<State> Create(std::weak_ptr<engine::AssetsManager> am, std::weak_ptr<engine::Renderer> r);

   private:
    engine::SceneManager m_sceneManager;
    std::weak_ptr<engine::Renderer> m_renderer;
    std::weak_ptr<engine::AssetsManager> m_assetsManager;
};
}
