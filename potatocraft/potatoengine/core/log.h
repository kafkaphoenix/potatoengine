#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace potatoengine {

class Log {
   public:
    static void Init();
    static std::shared_ptr<spdlog::logger> &GetLogger() noexcept { return s_logger; }

   private:
    inline static std::shared_ptr<spdlog::logger> s_logger;
};

}

namespace engine = potatoengine;

#define CORE_TRACE(...) ::engine::Log::GetLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::engine::Log::GetLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::engine::Log::GetLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::engine::Log::GetLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::engine::Log::GetLogger()->critical(__VA_ARGS__)