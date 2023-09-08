#pragma once

#include "potatoengine/pch.h"

namespace potatoengine::assets {

constexpr inline auto enum_range = [](auto front, auto back) {
    return std::views::iota(std::to_underlying(front), std::to_underlying(back) + 1) | std::views::transform([](auto e) { return decltype(front)(e); });
};

enum class TextureID {
    Default,
    Cube,
    BlockAtlas,
};

enum class ShaderID {
    VBasic,
    FBasic,
};

enum class ModelID {
    Rock,
    Backpack,
    BrickBlock,
    StoneBlock,
    GlassBlock,
    WoodBlock,
};

enum class PrefabID {
    Player,
    Chicken,
    BrickBlock,
    StoneBlock,
    GlassBlock,
    WoodBlock,
};

enum class SoundID {
    Default,
};

enum class FontID {
    Default,
};

}