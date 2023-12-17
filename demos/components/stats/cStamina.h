#pragma once

namespace demos {

struct CStamina {
  int base{};
  int current{};

  CStamina() = default;
  explicit CStamina(int b) : base(b), current(b) {}

  void print() const { APP_BACKTRACE("\t\tbase: {0}\n\t\t\t\tcurrent: {1}", base, current); }
};
}