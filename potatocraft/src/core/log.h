#pragma once

#include "src/core/base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace potatoengine
{

  class Log
  {
  public:
    static void Init();

    static Ref<spdlog::logger> &GetCoreLogger() { return s_coreLogger; }

  private:
    static Ref<spdlog::logger> s_coreLogger;
  };

}

// Core log macros
#define CORE_TRACE(...) ::potatoengine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::potatoengine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::potatoengine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::potatoengine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::potatoengine::Log::GetCoreLogger()->critical(__VA_ARGS__)