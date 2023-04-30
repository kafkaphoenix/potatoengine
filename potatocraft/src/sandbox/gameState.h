#pragma once

#include "src/engineAPI.h"

namespace potatocraft {

    class GameState : public potatoengine::State 
    {
    public:
        GameState();
        virtual ~GameState() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onUpdate(potatoengine::Timestep ts) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(potatoengine::Event& event) override;

    private:
        potatoengine::Ref<potatoengine::Program> m_shader;
        potatoengine::Ref<potatoengine::VAO> m_vao;
        potatoengine::Ref<potatoengine::Texture> m_texture;
        potatoengine::Camera m_camera;
    };
}
