#pragma once

#include "engineAPI.h"

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
      : strength(s), dexterity(d), constitution(c), intelligence(i), wisdom(w),
        charisma(ch) {}

    void print() const {
      APP_BACKTRACE("\t\tstrength: {0}\n\t\t\t\tdexterity: "
                    "{1}\n\t\t\t\tconstitution: {2}\n\t\t\t\tintelligence: "
                    "{3}\n\t\t\t\twisdom: {4}\n\t\t\t\tcharisma: {5}",
                    strength, dexterity, constitution, intelligence, wisdom,
                    charisma);
    }

    std::map<std::string, std::string, engine::NumericComparator> getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["strength"] = std::to_string(strength);
      info["dexterity"] = std::to_string(dexterity);
      info["constitution"] = std::to_string(constitution);
      info["intelligence"] = std::to_string(intelligence);
      info["wisdom"] = std::to_string(wisdom);
      info["charisma"] = std::to_string(charisma);

      return info;
    }
};
}
