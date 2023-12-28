#pragma once

#include <imgui.h>
#pragma warning(push, 0)
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <memory>
#include <shared_mutex> // read-write lock (many readers allowed, but writing must be exclusive)
#include <string>
#include <vector>

namespace potatoengine::ui {
class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex> {
  public:
    void Draw(bool* show_tool_logger);
    void Clear();
    void ToggleAutoScroll() { m_autoScroll = not m_autoScroll; }
    void ToggleWrap() { m_wrap = not m_wrap; }

  protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

  private:
    void showLogFormatPopup();
    struct Record {
        std::string m_time;
        std::string m_thread;
        std::string m_source;
        std::string m_level;
        std::string m_message;
    };
    int calculateElapsedLocalTime(const Record& record);
    std::vector<Record> m_records;
    mutable std::shared_timed_mutex m_recordsMutex;

    bool m_autoScroll{};
    bool m_wrap{};

    bool m_showTime{true};
    bool m_showThread{true};
    bool m_showSource{true};
    bool m_showLevel{true};
    std::vector<std::string> m_levels{"all",     "trace", "debug",   "info",
                                      "warning", "error", "critical"};
    std::string m_filterLevel{"all"};
    std::vector<std::string> m_times{"all", "1s",  "5s",  "10s", "30s",
                                     "1m",  "5m",  "10m", "30m", "1h",
                                     "5h",  "10h", "30h"};
    std::string m_filterTime{"all"};
    char m_textFilter[128]{};
};
}