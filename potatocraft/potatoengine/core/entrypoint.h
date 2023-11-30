#pragma once

#include "potatoengine/core/application.h"
#include "potatoengine/utils/exception.h"

namespace engine = potatoengine;

extern engine::Application *engine::CreateApp(engine::CLArgs &&args);

int main(int argc, char **argv) {
    try {
        engine::LogManager::Init();

        std::vector<const char *> args(argv, argv + argc);
        engine::CLArgs clargs{std::span<const char *>{args}};
        engine::Application *app = engine::CreateApp(std::move(clargs));

        app->run();

        delete app;
    } catch (const engine::EngineException &e) {
        ENGINE_CRITICAL(e.what());
    } catch (const engine::AppException &e) {
        APP_CRITICAL(e.what());
    } catch (const std::exception &e) {
        APP_CRITICAL(e.what()); // We do not know the source of the exception, so we assume it is from the app
    }
}