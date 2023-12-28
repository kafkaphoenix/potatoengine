#pragma once

#include "engineAPI.h"
namespace demos {

class GameState : public engine::State {
  public:
    GameState(std::weak_ptr<engine::AssetsManager> am,
              std::weak_ptr<engine::Renderer> r,
              std::weak_ptr<engine::Settings> s,
              std::weak_ptr<engine::SceneManager> sm);

    virtual void onAttach() override final;
    virtual void onDetach() override final;
    virtual void onUpdate(engine::Time ts) override final;
    virtual void onImguiUpdate() override final;
    virtual void onEvent(engine::Event& e) override final;

    static std::unique_ptr<State>
    Create(std::weak_ptr<engine::AssetsManager> am,
           std::weak_ptr<engine::Renderer> r, std::weak_ptr<engine::Settings> s,
           std::weak_ptr<engine::SceneManager> sm);

  private:
    std::weak_ptr<engine::Renderer> m_renderer;
    std::weak_ptr<engine::AssetsManager> m_assetsManager;
    std::weak_ptr<engine::Settings> m_settings;
    std::weak_ptr<engine::SceneManager> m_sceneManager;
};
}
