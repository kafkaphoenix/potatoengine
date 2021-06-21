#pragma once

#include <src/core/base.hpp>

int main(int argc, char** argv);

namespace potatocraft {

    struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const {
			return args[index];
		}
	};

    class Application
    {
    public:
        Application(const std::string &name, ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        ~Application();

        ApplicationCommandLineArgs getCommandLineArgs() const { return m_commandLineArgs; }

    private:
        void run();

        ApplicationCommandLineArgs m_commandLineArgs;

        friend int ::main(int argc, char** argv);
    };
}