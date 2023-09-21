#pragma once

namespace potatocraft {

struct CExperience {
    int current{};

    CExperience() = default;
    explicit CExperience(int c) : current(c) {}

    void print() const {
        CORE_INFO("\t\tcurrent: {0}", current);
    }
};
}