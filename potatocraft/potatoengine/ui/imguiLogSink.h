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
    void Draw();
    void Clear();
    void ToggleAutoScroll() { m_autoScroll = !m_autoScroll; }
    void ToggleWrap() { m_wrap = !m_wrap; }

  protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

  private:
    struct Record {
        std::string m_source;
        std::string m_level;
        std::string m_message;
        bool m_emphasize{};
    };
    std::vector<Record> m_records;
    mutable std::shared_timed_mutex m_recordsMutex;
    ImGuiTextFilter m_filter;

    bool m_autoScroll{};
    bool m_wrap{};

    bool m_showTime{true};
    bool m_showThread{true};
    bool m_showLogSource{true};
    bool m_showLogLevel{true};
};
}