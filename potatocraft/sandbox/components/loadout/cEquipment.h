#pragma once

#include "potatoengine/engineAPI.h"

namespace potatocraft {

struct CEquipment {
    std::string head{};
    std::string neck{};
    std::string shoulders{};
    std::string chest{};
    std::string back{};
    std::string hands{};
    std::string lfinger{};
    std::string rfinger{};
    std::string belt{};
    std::string legs{};
    std::string feet{};

    CEquipment() = default;
    explicit CEquipment(std::string&& h, std::string&& n, std::string&& s, std::string&& c, std::string&& b, std::string&& ha, std::string&& lf, std::string&& rf, std::string&& be, std::string&& l, std::string&& f)
        : head(std::move(h)), neck(std::move(n)), shoulders(std::move(s)), chest(std::move(c)), back(std::move(b)), hands(std::move(ha)), lfinger(std::move(lf)), rfinger(std::move(rf)), belt(std::move(be)), legs(std::move(l)), feet(std::move(f)) {}

    void print() const {
        CORE_INFO("\t\thead: {0}\n\t\t\t\tneck: {1}\n\t\t\t\tshoulders: {2}\n\t\t\t\tchest: {3}\n\t\t\t\tback: {4}\n\t\t\t\thands: {5}\n\t\t\t\tlfinger: {6}\n\t\t\t\trfinger: {7}\n\t\t\t\tbelt: {8}\n\t\t\t\tlegs: {9}\n\t\t\t\tfeet: {10}", head, neck, shoulders, chest, back, hands, lfinger, rfinger, belt, legs, feet);
    }
};
}

template <>
void engine::SceneManager::onComponentAdded<potatocraft::CEquipment>(engine::Entity e, potatocraft::CEquipment& c) { // TODO finish
}