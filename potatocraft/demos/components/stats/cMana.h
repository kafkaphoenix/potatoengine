#pragma once

namespace potatocraft {

struct CMana {
    int base{};
    int current{};

    CMana() = default;
    explicit CMana(int b) : base(b), current(b) {}

    void print() const {
        CORE_TRACE("\t\tbase: {0}\n\t\t\t\tcurrent: {1}", base, current);
    }
};
}