#pragma once

#include "src/core/base.hpp"
#include "src/core/window.hpp"

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

        //void onEvent(Event& e);
		//void pushLayer(Layer* layer);
		//void pushOverlay(Layer* layer);

        Window& GetWindow() { return *m_window; }

        ApplicationCommandLineArgs getCommandLineArgs() const { return m_commandLineArgs; }

    private:
        Scope<Window> m_window;

        void run();

        //bool OnWindowClose(WindowCloseEvent& e);
		//bool OnWindowResize(WindowResizeEvent& e);

        ApplicationCommandLineArgs m_commandLineArgs;

        friend int ::main(int argc, char **argv);
    };
}