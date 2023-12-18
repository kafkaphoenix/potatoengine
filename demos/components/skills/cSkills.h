#pragma once

#include "engineAPI.h"

namespace demos {

struct CSkills {
    int mining{};
    int jewelcrafting{};
    int blacksmithing{};
    int hunting{};
    int fishing{};
    int skinning{};
    int leatherworking{};
    int herbalism{};
    int cooking{};
    int alchemy{};
    int enchanting{};
    int harvesting{};
    int tailoring{};
    int woodworking{};
    int woodcutting{};
    int farming{};
    int quarrying{};
    int masonry{};

    CSkills() = default;
    explicit CSkills(int m, int j, int b, int h, int f, int sk, int l, int he,
                     int c, int a, int e, int ha, int t, int w, int wc, int fa,
                     int q, int ma)
      : mining(m), jewelcrafting(j), blacksmithing(b), hunting(h), fishing(f),
        skinning(sk), leatherworking(l), herbalism(he), cooking(c), alchemy(a),
        enchanting(e), harvesting(ha), tailoring(t), woodworking(w),
        woodcutting(wc), farming(fa), quarrying(q), masonry(ma) {}

    void print() const {
      APP_BACKTRACE("\t\tmining: {0}\n\t\t\t\tjewelcrafting: "
                    "{1}\n\t\t\t\tblacksmithing: {2}\n\t\t\t\thunting: "
                    "{3}\n\t\t\t\tfishing: {4}\n\t\t\t\tskinning: "
                    "{5}\n\t\t\t\tleatherworking: {6}\n\t\t\t\therbalism: "
                    "{7}\n\t\t\t\tcooking: {8}\n\t\t\t\talchemy: "
                    "{9}\n\t\t\t\tenchanting: {10}\n\t\t\t\tharvesting: "
                    "{11}\n\t\t\t\ttailoring: {12}\n\t\t\t\twoodworking: "
                    "{13}\n\t\t\t\twoodcutting: {14}\n\t\t\t\tfarming: "
                    "{15}\n\t\t\t\tquarrying: {16}\n\t\t\t\tmasonry: {17}",
                    mining, jewelcrafting, blacksmithing, hunting, fishing,
                    skinning, leatherworking, herbalism, cooking, alchemy,
                    enchanting, harvesting, tailoring, woodworking, woodcutting,
                    farming, quarrying, masonry);
    }

    std::map<std::string, std::string, engine::NumericComparator>
    getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["mining"] = std::to_string(mining);
      info["jewelcrafting"] = std::to_string(jewelcrafting);
      info["blacksmithing"] = std::to_string(blacksmithing);
      info["hunting"] = std::to_string(hunting);
      info["fishing"] = std::to_string(fishing);
      info["skinning"] = std::to_string(skinning);
      info["leatherworking"] = std::to_string(leatherworking);
      info["herbalism"] = std::to_string(herbalism);
      info["cooking"] = std::to_string(cooking);
      info["alchemy"] = std::to_string(alchemy);
      info["enchanting"] = std::to_string(enchanting);
      info["harvesting"] = std::to_string(harvesting);
      info["tailoring"] = std::to_string(tailoring);
      info["woodworking"] = std::to_string(woodworking);
      info["woodcutting"] = std::to_string(woodcutting);
      info["farming"] = std::to_string(farming);
      info["quarrying"] = std::to_string(quarrying);
      info["masonry"] = std::to_string(masonry);

      return info;
    }
};
}