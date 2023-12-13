#pragma once

namespace demos {

struct CStats {
  int strength{};
  int dexterity{};
  int constitution{};
  int intelligence{};
  int wisdom{};
  int charisma{};

  CStats() = default;
  explicit CStats(int s, int d, int c, int i, int w, int ch)
      : strength(s), dexterity(d), constitution(c), intelligence(i), wisdom(w), charisma(ch) {}

  void print() const {
    APP_TRACE("\t\tstrength: {0}\n\t\t\t\tdexterity: {1}\n\t\t\t\tconstitution: {2}\n\t\t\t\tintelligence: "
              "{3}\n\t\t\t\twisdom: {4}\n\t\t\t\tcharisma: {5}",
              strength, dexterity, constitution, intelligence, wisdom, charisma);
  }
};
}
