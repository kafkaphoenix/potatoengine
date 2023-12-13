#pragma once

#include "pch.h"

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
      ENGINE_TRACE("\t\tfilepath: {0}\n\tvolume: {1}\n\tpitch: {2}\n\tloop: {3}", filepath, volume, pitch, loop);
    }
};
}