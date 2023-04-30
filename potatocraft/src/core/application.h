#pragma once

#include "src/core/base.h"
#include "src/core/window.h"
#include "src/event/applicationEvent.h"
#include "src/event/keyEvent.h"
#include "src/core/stateStack.h"
#include "src/core/state.h"

int main(int argc, char **argv);

namespace potatoengine
{

    struct CommandLineArgs
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
        Application(const std::string &name, CommandLineArgs args);
        virtual ~Application();

        void onEvent(Event& e);
		void pushState(State* state);
		void pushOverlay(State* state);

        Window& getWindow() { return *m_window; }

        void close();

        static Application& Get() { return *s_instance; }

    private:
        void run();
        bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
		bool onKeyPressed(KeyPressedEvent& e); //todo move

        Scope<Window> m_window;
        bool m_running = true;
		bool m_minimized = false;
		bool m_debugging = false;//todo move
		bool m_wireframe = false;//todo move
        float m_lastFrame{};
        float m_accumulator{};
        StateStack m_stateStack;

        std::string m_name;
        CommandLineArgs m_commandLineArgs;

        inline static Application* s_instance = nullptr;
        friend int ::main(int argc, char **argv);
    };

    // To be defined in CLIENT
	Application* CreateApp(const std::string &name, CommandLineArgs args);
}