#pragma once

#include "src/core/base.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace potatocraft {

    class Log
    {
    public:
        static void init();

        static Ref<spdlog::logger>& getCoreLogger() { return s_coreLogger; }

    private:
		static Ref<spdlog::logger> s_coreLogger;
    };

}

// Core log macros
#define CORE_TRACE(...)    ::potatocraft::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::potatocraft::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::potatocraft::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::potatocraft::Log::getCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::potatocraft::Log::getCoreLogger()->critical(__VA_ARGS__)