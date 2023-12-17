#pragma once

namespace demos {

struct CHealth {
  int base{};
  int current{};

  CHealth() = default;
  explicit CHealth(int b) : base(b), current(b) {}

  void print() const { APP_BACKTRACE("\t\tbase: {0}\n\t\t\t\tcurrent: {1}", base, current); }
};
}