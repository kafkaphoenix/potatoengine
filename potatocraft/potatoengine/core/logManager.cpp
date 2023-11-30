#include "potatoengine/core/logManager.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "potatoengine/ui/imguiLogSink.h"

namespace potatoengine {

void LogManager::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.reserve(2);
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<ui::ImGuiLogSink>());

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");

    s_engineLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_engineLogger);
    s_engineLogger->set_level(spdlog::level::trace);
    s_engineLogger->flush_on(spdlog::level::trace);

    s_appLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_appLogger);
    s_appLogger->set_level(spdlog::level::trace);
    s_appLogger->flush_on(spdlog::level::trace);
}

void LogManager::CreateFileLogger(std::string_view filepath) {
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath.data(), true);

    fileSink->set_pattern("[%T] [%l] %n: %v");

    s_appLogger->sinks().emplace_back(fileSink);
}
}