#pragma once

#include "src/core/base.hpp"
#include "src/core/window.hpp"
#include "src/event/applicationEvent.hpp"
#include "src/event/keyEvent.hpp"
#include "src/core/stateStack.hpp"
#include "src/core/state.hpp"

int main(int argc, char **argv);

namespace potatocraft
{

    struct ApplicationCommandLineArgs
    {
        int count = 0;
        char **args = nullptr;

        const char *operator[](int index) const
        {
            return args[index];
        }
    };

    class Application
    {
    public:
        Application(const std::string &name, ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        ~Application();

        void onEvent(Event& e);
		void pushState(State* state);
		void pushOverlay(State* state);

        Window& getWindow() { return *m_window; }

        void close();

        static Application& get() { return *s_instance; }

        ApplicationCommandLineArgs getCommandLineArgs() const { return m_commandLineArgs; }

    private:
        void run();
        bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);

        Scope<Window> m_window;
        bool m_running = true;
		bool m_minimized = false;
		bool m_debugging = false;
		bool m_wireframe = false;
        float m_lastFrame = 0.f;
        float m_accumulator = 0.f;
        StateStack m_stateStack;

        ApplicationCommandLineArgs m_commandLineArgs;

        static Application* s_instance;
        friend int ::main(int argc, char **argv);
    };
}