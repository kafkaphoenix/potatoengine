#pragma once

namespace potatoengine {

struct CBlock {
    enum class Type {
      Air,
      Dirt,
      Grass,
      Stone,
      Water,
      Wood,
      Leaves,
      Sand,
      Snow,
      Ice,
      Cloud,
      Lava,
      Bedrock,
      Cactus,
      Sandstone,
    };

    std::string _type{"air"};
    Type type{Type::Air};

    CBlock() = default;
    explicit CBlock(std::string&& t) : _type(std::move(t)) {}

    void print() const { ENGINE_BACKTRACE("\t\ttype: {0}", _type); }

    void setBlockType() {
      if (_type == "air") {
        type = Type::Air;
      } else if (_type == "dirt") {
        type = Type::Dirt;
      } else if (_type == "grass") {
        type = Type::Grass;
      } else if (_type == "stone") {
        type = Type::Stone;
      } else if (_type == "water") {
        type = Type::Water;
      } else if (_type == "wood") {
        type = Type::Wood;
      } else if (_type == "leaves") {
        type = Type::Leaves;
      } else if (_type == "sand") {
        type = Type::Sand;
      } else if (_type == "snow") {
        type = Type::Snow;
      } else if (_type == "ice") {
        type = Type::Ice;
      } else if (_type == "cloud") {
        type = Type::Cloud;
      } else if (_type == "lava") {
        type = Type::Lava;
      } else if (_type == "bedrock") {
        type = Type::Bedrock;
      } else if (_type == "cactus") {
        type = Type::Cactus;
      } else if (_type == "sandstone") {
        type = Type::Sandstone;
      } else {
        ENGINE_ASSERT(false, "Unknown block type {}", _type);
      }
    }
};
}
