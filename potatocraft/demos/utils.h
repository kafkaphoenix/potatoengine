#pragma once

#include <entt/entt.hpp>

#include "potatoengine/engineAPI.h"
#include "demos/components/inventory/cInventory.h"
#include "demos/components/inventory/cItem.h"
#include "demos/components/loadout/cEquipment.h"
#include "demos/components/skills/cSkills.h"
#include "demos/components/stats/cExperience.h"
#include "demos/components/stats/cHealth.h"
#include "demos/components/stats/cMana.h"
#include "demos/components/stats/cStamina.h"
#include "demos/components/stats/cStats.h"
#include "demos/components/stats/cTalents.h"


namespace potatocraft {

CItem &castCItem(void *other) {
    return *static_cast<CItem *>(other);
}

CInventory &castCInventory(void *other) {
    return *static_cast<CInventory *>(other);
}

CHealth &castCHealth(void *other) {
    return *static_cast<CHealth *>(other);
}

CMana &castCMana(void *other) {
    return *static_cast<CMana *>(other);
}

CStamina &castCStamina(void *other) {
    return *static_cast<CStamina *>(other);
}

CExperience &castCExperience(void *other) {
    return *static_cast<CExperience *>(other);
}

CEquipment &castCEquipment(void *other) {
    return *static_cast<CEquipment *>(other);
}

CStats &castCStats(void *other) {
    return *static_cast<CStats *>(other);
}

CTalents &castCTalents(void *other) {
    return *static_cast<CTalents *>(other);
}

CSkills &castCSkills(void *other) {
    return *static_cast<CSkills *>(other);
}

void registerComponents() {
    using namespace entt::literals;

    entt::meta<CItem>()  // TODO probably this is not a component?
        .type("item"_hs)
        .ctor<&castCItem, entt::as_ref_t>()
        .data<&CItem::name>("name"_hs)
        .data<&CItem::description>("description"_hs)
        .data<&CItem::icon>("icon"_hs)
        .data<&CItem::model>("model"_hs)
        .data<&CItem::value>("value"_hs)
        .func<&CItem::print>("print"_hs)
        .func<&engine::assign<CItem>, entt::as_ref_t>("assign"_hs);

    entt::meta<CInventory>()
        .type("inventory"_hs)
        .ctor<&castCInventory, entt::as_ref_t>()
        .data<&CInventory::items>("items"_hs) // TODO modify assign rethink initialization
        .func<&CInventory::print>("print"_hs)
        .func<&engine::assign<CInventory>, entt::as_ref_t>("assign"_hs);

    entt::meta<CHealth>()
        .type("health"_hs)
        .ctor<&castCHealth, entt::as_ref_t>()
        .data<&CHealth::base>("base"_hs)
        .data<&CHealth::current>("current"_hs)
        .func<&CHealth::print>("print"_hs)
        .func<&engine::assign<CHealth>, entt::as_ref_t>("assign"_hs);

    entt::meta<CMana>()
        .type("mana"_hs)
        .ctor<&castCMana, entt::as_ref_t>()
        .data<&CMana::base>("base"_hs)
        .data<&CMana::current>("current"_hs)
        .func<&CMana::print>("print"_hs)
        .func<&engine::assign<CMana>, entt::as_ref_t>("assign"_hs);

    entt::meta<CStamina>()
        .type("stamina"_hs)
        .ctor<&castCStamina, entt::as_ref_t>()
        .data<&CStamina::base>("base"_hs)
        .data<&CStamina::current>("current"_hs)
        .func<&CStamina::print>("print"_hs)
        .func<&engine::assign<CStamina>, entt::as_ref_t>("assign"_hs);

    entt::meta<CExperience>()
        .type("experience"_hs)
        .ctor<&castCExperience, entt::as_ref_t>()
        .data<&CExperience::current>("current"_hs)
        .func<&CExperience::print>("print"_hs)
        .func<&engine::assign<CExperience, int>, entt::as_ref_t>("assign"_hs);

    entt::meta<CEquipment>()
        .type("equipment"_hs)
        .ctor<&castCEquipment, entt::as_ref_t>()
        .data<&CEquipment::head>("head"_hs)
        .data<&CEquipment::neck>("neck"_hs)
        .data<&CEquipment::shoulders>("shoulders"_hs)
        .data<&CEquipment::chest>("chest"_hs)
        .data<&CEquipment::back>("back"_hs)
        .data<&CEquipment::hands>("hands"_hs)
        .data<&CEquipment::lfinger>("lfinger"_hs)
        .data<&CEquipment::rfinger>("rfinger"_hs)
        .data<&CEquipment::belt>("belt"_hs)
        .data<&CEquipment::legs>("legs"_hs)
        .data<&CEquipment::feet>("feet"_hs)
        .func<&CEquipment::print>("print"_hs)
        .func<&engine::onComponentAdded<CEquipment>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&engine::assign<CEquipment>, entt::as_ref_t>("assign"_hs);

    entt::meta<CStats>()
        .type("stats"_hs)
        .ctor<&castCStats, entt::as_ref_t>()
        .data<&CStats::strength>("strength"_hs)
        .data<&CStats::dexterity>("dexterity"_hs)
        .data<&CStats::constitution>("constitution"_hs)
        .data<&CStats::intelligence>("intelligence"_hs)
        .data<&CStats::wisdom>("wisdom"_hs)
        .data<&CStats::charisma>("charisma"_hs)
        .func<&CStats::print>("print"_hs)
        .func<&engine::assign<CStats>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTalents>()
        .type("talents"_hs)
        .ctor<&castCTalents, entt::as_ref_t>()
        .data<&CTalents::acrobatics>("acrobatics"_hs)
        .data<&CTalents::arcana>("arcana"_hs)
        .data<&CTalents::athletics>("athletics"_hs)
        .data<&CTalents::perception>("perception"_hs)
        .data<&CTalents::persuasion>("persuasion"_hs)
        .data<&CTalents::stealth>("stealth"_hs)
        .data<&CTalents::survival>("survival"_hs)
        .data<&CTalents::luck>("luck"_hs)
        .func<&CTalents::print>("print"_hs)
        .func<&engine::assign<CTalents>, entt::as_ref_t>("assign"_hs);

    entt::meta<CSkills>()
        .type("skills"_hs)
        .ctor<&castCSkills, entt::as_ref_t>()
        .data<&CSkills::mining>("mining"_hs)
        .data<&CSkills::jewelcrafting>("jewelcrafting"_hs)
        .data<&CSkills::blacksmithing>("blacksmithing"_hs)
        .data<&CSkills::fishing>("fishing"_hs)
        .data<&CSkills::hunting>("hunting"_hs)
        .data<&CSkills::skinning>("skinning"_hs)
        .data<&CSkills::leatherworking>("leatherworking"_hs)
        .data<&CSkills::herbalism>("herbalism"_hs)
        .data<&CSkills::cooking>("cooking"_hs)
        .data<&CSkills::alchemy>("alchemy"_hs)
        .data<&CSkills::enchanting>("enchanting"_hs)
        .data<&CSkills::harvesting>("harvesting"_hs)
        .data<&CSkills::tailoring>("tailoring"_hs)
        .data<&CSkills::woodworking>("woodworking"_hs)
        .data<&CSkills::woodcutting>("woodcutting"_hs)
        .data<&CSkills::farming>("farming"_hs)
        .data<&CSkills::quarrying>("quarrying"_hs)
        .data<&CSkills::masonry>("masonry"_hs)
        .func<&CSkills::print>("print"_hs)
        .func<&engine::assign<CSkills>, entt::as_ref_t>("assign"_hs);
}
}