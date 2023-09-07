#pragma once

#include "potatoengine/pch.h"

namespace potatoengine::assets {

inline std::string to_string(assets::TextureID id) noexcept {
    using enum assets::TextureID;
    switch (id) {
        case Default:
            return "Default";
        case Cube:
            return "Cube";
    }
}

inline std::string to_string(assets::ShaderID id) noexcept {
    using enum assets::ShaderID;
    switch (id) {
        case VBasic:
            return "VBasic";
        case FBasic:
            return "FBasic";
    }
}

inline std::string to_string(assets::ModelID id) noexcept {
    using enum assets::ModelID;
    switch (id) {
        case Rock:
            return "Rock";
        case Backpack:
            return "Backpack";
        case Cube:
            return "Cube";
    }
}

inline std::string to_string(assets::PrefabID id) noexcept {
    using enum assets::PrefabID;
    switch (id) {
        case Player:
            return "Player";
        case Chicken:
            return "Chicken";
        case BrickBlock:
            return "BrickBlock";
        case StoneBlock:
            return "StoneBlock";
    }
}

inline std::string to_string(assets::SoundID id) noexcept {
    using enum assets::SoundID;
    switch (id) {
        case Default:
            return "Default";
    }
}

inline std::string to_string(assets::FontID id) noexcept {
    using enum assets::FontID;
    switch (id) {
        case Default:
            return "Default";
    }
}
}
