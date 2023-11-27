#pragma once

namespace potatocraft {

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
    explicit CTalents(int a, int ar, int at, int p, int pe, int s, int su, int l)
        : acrobatics(a), arcana(ar), athletics(at), perception(p), persuasion(pe), stealth(s), survival(su), luck(l) {}

    void print() const {
        CORE_TRACE("\t\tacrobatics: {0}\n\t\t\t\tarcana: {1}\n\t\t\t\tathletics: {2}\n\t\t\t\tperception: {3}\n\t\t\t\tpersuasion: {4}\n\t\t\t\tstealth: {5}\n\t\t\t\tsurvival: {6}\n\t\t\t\tluck: {7}",
            acrobatics, arcana, athletics, perception, persuasion, stealth, survival, luck);
    }
};
}