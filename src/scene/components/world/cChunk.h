#pragma once

#include "scene/components/graphics/cMesh.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/world/cBlock.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CChunk {
    enum class Biome {
      desert,
      forest,
      jungle,
      plains,
      savanna,
      snowy,
      swamp,
      taiga,
    };

    std::string _biome{"plains"};
    Biome biome{Biome::plains};
    std::vector<CBlock> blocks;
    CMesh terrainMesh;
    CTransform transform;

    CChunk() = default;
    explicit CChunk(std::string&& b) : _biome(std::move(b)) {}

    void print() const {
      std::string b;
      for (int i = 0; i < blocks.size(); ++i) {
        b += std::format("\n\t\t\t\t\tblock: {} {}", i, blocks[i]._type);
      }
      ENGINE_BACKTRACE("\t\tbiome: {0}\n\t\t\t\tblocks: {1}", _biome, b);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["biome"] = _biome;
      info["mesh 0"] = "0";
      info["transform 0"] = "0";

      return info;
    }

    std::map<std::string, std::string, NumericComparator>
    getMeshInfo(int index) const {
      return terrainMesh.getInfo();
    }

    std::map<std::string, std::string, NumericComparator>
    getTransformInfo(int index) const {
      return transform.getInfo();
    }

    void setBiome() {
      if (_biome == "desert") {
        biome = Biome::desert;
      } else if (_biome == "forest") {
        biome = Biome::forest;
      } else if (_biome == "jungle") {
        biome = Biome::jungle;
      } else if (_biome == "plains") {
        biome = Biome::plains;
      } else if (_biome == "savanna") {
        biome = Biome::savanna;
      } else if (_biome == "snowy") {
        biome = Biome::snowy;
      } else if (_biome == "swamp") {
        biome = Biome::swamp;
      } else if (_biome == "taiga") {
        biome = Biome::taiga;
      }
    }
};
}
