#pragma once

namespace potatoengine {

struct CTextureAtlas {
    int rows{};
    int index{};

    CTextureAtlas() = default;
    explicit CTextureAtlas(int r, int i) : rows(r), index(i) {}

    void print() const {
        ENGINE_TRACE("\t\trows: {0}\n\t\t\t\tindex: {1}", rows, index);
    }
};
}