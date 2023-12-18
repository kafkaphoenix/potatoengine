#pragma once

#include "engineAPI.h"

namespace demos {

struct CTalents {
    int acrobatics{};
    int arcana{};
    int athletics{};
    int perception{};
    int persuasion{};
    int stealth{};
    int survival{};
    int luck{};

    CTalents() = default;
    explicit CTalents(int a, int ar, int at, int p, int pe, int s, int su,
                      int l)
      : acrobatics(a), arcana(ar), athletics(at), perception(p), persuasion(pe),
        stealth(s), survival(su), luck(l) {}

    void print() const {
      APP_BACKTRACE("\t\tacrobatics: {0}\n\t\t\t\tarcana: "
                    "{1}\n\t\t\t\tathletics: {2}\n\t\t\t\tperception: "
                    "{3}\n\t\t\t\tpersuasion: {4}\n\t\t\t\tstealth: "
                    "{5}\n\t\t\t\tsurvival: {6}\n\t\t\t\tluck: {7}",
                    acrobatics, arcana, athletics, perception, persuasion,
                    stealth, survival, luck);
    }

    std::map<std::string, std::string, engine::NumericComparator> getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["acrobatics"] = std::to_string(acrobatics);
      info["arcana"] = std::to_string(arcana);
      info["athletics"] = std::to_string(athletics);
      info["perception"] = std::to_string(perception);
      info["persuasion"] = std::to_string(persuasion);
      info["stealth"] = std::to_string(stealth);
      info["survival"] = std::to_string(survival);
      info["luck"] = std::to_string(luck);

      return info;
    }
};
}