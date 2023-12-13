#pragma once

namespace potatoengine {

class Time {
  public:
    Time(float time = 0.f) : m_time(time) {}

    operator float() const noexcept { return m_time; }

    float getSeconds() const noexcept { return m_time; }
    float getMilliseconds() const noexcept { return m_time * 1000.f; }

  private:
    float m_time{};
};

}