#include "ui/imguiLogSink.h"

#include <date/date.h>
#include <sstream>

extern template class spdlog::sinks::base_sink<std::mutex>;
namespace potatoengine::ui {

void ImGuiLogSink::Clear() {
  std::unique_lock<std::shared_timed_mutex> lock(m_recordsMutex);
  m_records.clear();
}

void ImGuiLogSink::Draw() {}

void ImGuiLogSink::sink_it_(const spdlog::details::log_msg& msg) {
  auto ostream = std::ostringstream{};

  if (m_showTime) {
    ostream << "[" << date::format("%D %T %Z", msg.time) << "] ";
  }

  if (m_showThread) {
    ostream << "[" << msg.thread_id << "] ";
  }

  if (m_showLogSource) {
    ostream.put('[').write(msg.logger_name.data(), msg.logger_name.size()).write("] ", 2);
  }

  if (m_showLogLevel) {
    ostream << "[" << spdlog::level::to_short_c_str(msg.level) << "] ";
  }

  auto metadata = ostream.str();
}

void ImGuiLogSink::flush_() {}

}