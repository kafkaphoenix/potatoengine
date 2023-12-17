#pragma once

namespace demos {

struct CExperience {
  int current{};

  CExperience() = default;
  explicit CExperience(int c) : current(c) {}

  void print() const { APP_BACKTRACE("\t\tcurrent: {}", current); }
};
}