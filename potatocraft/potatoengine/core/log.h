#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "potatoengine/pch.h"

namespace potatoengine {

class Log {
   public:
    static void Init();

    static std::shared_ptr<spdlog::logger> &GetLogger() { return s_logger; }

   private:
    inline static std::shared_ptr<spdlog::logger> s_logger;
};

}

// Core log macros
#define CORE_TRACE(...) ::potatoengine::Log::GetLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::potatoengine::Log::GetLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::potatoengine::Log::GetLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::potatoengine::Log::GetLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::potatoengine::Log::GetLogger()->critical(__VA_ARGS__)