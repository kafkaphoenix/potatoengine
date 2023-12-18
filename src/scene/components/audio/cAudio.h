#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CAudio {
    // TODO Audio class
    std::string filepath{};
    float volume{};
    float pitch{};
    bool loop{};

    CAudio() = default;
    explicit CAudio(std::string&& fp, float v, float p, bool l)
      : filepath(std::move(fp)), volume(v), pitch(p), loop(l) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\tfilepath: {0}\n\tvolume: {1}\n\tpitch: {2}\n\tloop: {3}", filepath,
        volume, pitch, loop);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["filepath"] = filepath;
      info["volume"] = std::to_string(volume);
      info["pitch"] = std::to_string(pitch);
      info["loop"] = std::to_string(loop);

      return info;
    }
};
}