#pragma once

namespace potatocraft {

struct CStamina {
    int base{};
    int current{};

    CStamina() = default;
    explicit CStamina(int b) : base(b), current(b) {}

    void print() const {
        CORE_TRACE("\t\tbase: {0}\n\t\t\t\tcurrent: {1}", base, current);
    }
};
}