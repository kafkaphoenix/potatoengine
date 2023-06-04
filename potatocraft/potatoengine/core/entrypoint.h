#pragma once
#include "potatoengine/core/application.h"

extern potatoengine::Application *potatoengine::CreateApp(const std::string &name, CommandLineArgs args);

int main(int argc, char **argv) {
    try {
        potatoengine::Log::Init();

        auto app = potatoengine::CreateApp("potatocraft", {argc, argv});

        app->run();

        delete app;
    } catch (const std::runtime_error& ex) {
        std::cout << "Exception caught: " << ex.what() << std::endl;
    }
}