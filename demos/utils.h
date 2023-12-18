#pragma once

#include <entt/entt.hpp>

#include "components/inventory/cInventory.h"
#include "components/inventory/cItem.h"
#include "components/loadout/cEquipment.h"
#include "components/skills/cSkills.h"
#include "components/stats/cExperience.h"
#include "components/stats/cHealth.h"
#include "components/stats/cMana.h"
#include "components/stats/cStamina.h"
#include "components/stats/cStats.h"
#include "components/stats/cTalents.h"
#include "engineAPI.h"

using namespace entt::literals;

namespace demos {

CItem& CastCItem(void* other) { return *static_cast<CItem*>(other); }

CInventory& CastCInventory(void* other) {
  return *static_cast<CInventory*>(other);
}

CHealth& CastCHealth(void* other) { return *static_cast<CHealth*>(other); }

CMana& CastCMana(void* other) { return *static_cast<CMana*>(other); }

CStamina& CastCStamina(void* other) { return *static_cast<CStamina*>(other); }

CExperience& CastCExperience(void* other) {
  return *static_cast<CExperience*>(other);
}

CEquipment& CastCEquipment(void* other) {
  return *static_cast<CEquipment*>(other);
}

CStats& CastCStats(void* other) { return *static_cast<CStats*>(other); }

CTalents& CastCTalents(void* other) { return *static_cast<CTalents*>(other); }

CSkills& CastCSkills(void* other) { return *static_cast<CSkills*>(other); }

void RegisterComponents() {
  entt::meta<CItem>() // TODO probably this is not a component?
    .type("item"_hs)
    .ctor<&CastCItem, entt::as_ref_t>()
    .data<&CItem::name>("name"_hs)
    .data<&CItem::description>("description"_hs)
    .data<&CItem::icon>("icon"_hs)
    .data<&CItem::model>("model"_hs)
    .data<&CItem::value>("value"_hs)
    .func<&CItem::print>("print"_hs)
    .func<&CItem::getInfo>("getInfo"_hs)
    .func<&engine::assign<CItem>, entt::as_ref_t>("assign"_hs);

  entt::meta<CInventory>()
    .type("inventory"_hs)
    .ctor<&CastCInventory, entt::as_ref_t>()
    .data<&CInventory::items>(
      "items"_hs) // TODO modify assign rethink initialization
    .func<&CInventory::print>("print"_hs)
    .func<&CInventory::getInfo>("getInfo"_hs)
    .func<&engine::assign<CInventory>, entt::as_ref_t>("assign"_hs);

  entt::meta<CHealth>()
    .type("health"_hs)
    .ctor<&CastCHealth, entt::as_ref_t>()
    .data<&CHealth::base>("base"_hs)
    .data<&CHealth::current>("current"_hs)
    .func<&CHealth::print>("print"_hs)
    .func<&CHealth::getInfo>("getInfo"_hs)
    .func<&engine::assign<CHealth>, entt::as_ref_t>("assign"_hs);

  entt::meta<CMana>()
    .type("mana"_hs)
    .ctor<&CastCMana, entt::as_ref_t>()
    .data<&CMana::base>("base"_hs)
    .data<&CMana::current>("current"_hs)
    .func<&CMana::print>("print"_hs)
    .func<&CMana::getInfo>("getInfo"_hs)
    .func<&engine::assign<CMana>, entt::as_ref_t>("assign"_hs);

  entt::meta<CStamina>()
    .type("stamina"_hs)
    .ctor<&CastCStamina, entt::as_ref_t>()
    .data<&CStamina::base>("base"_hs)
    .data<&CStamina::current>("current"_hs)
    .func<&CStamina::print>("print"_hs)
    .func<&CStamina::getInfo>("getInfo"_hs)
    .func<&engine::assign<CStamina>, entt::as_ref_t>("assign"_hs);

  entt::meta<CExperience>()
    .type("experience"_hs)
    .ctor<&CastCExperience, entt::as_ref_t>()
    .data<&CExperience::current>("current"_hs)
    .func<&CExperience::print>("print"_hs)
    .func<&CExperience::getInfo>("getInfo"_hs)
    .func<&engine::assign<CExperience, int>, entt::as_ref_t>("assign"_hs);

  entt::meta<CEquipment>()
    .type("equipment"_hs)
    .ctor<&CastCEquipment, entt::as_ref_t>()
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
    .func<&CEquipment::getInfo>("getInfo"_hs)
    .func<&engine::onComponentAdded<CEquipment>, entt::as_ref_t>(
      "onComponentAdded"_hs)
    .func<&engine::assign<CEquipment>, entt::as_ref_t>("assign"_hs);

  entt::meta<CStats>()
    .type("stats"_hs)
    .ctor<&CastCStats, entt::as_ref_t>()
    .data<&CStats::strength>("strength"_hs)
    .data<&CStats::dexterity>("dexterity"_hs)
    .data<&CStats::constitution>("constitution"_hs)
    .data<&CStats::intelligence>("intelligence"_hs)
    .data<&CStats::wisdom>("wisdom"_hs)
    .data<&CStats::charisma>("charisma"_hs)
    .func<&CStats::print>("print"_hs)
    .func<&CStats::getInfo>("getInfo"_hs)
    .func<&engine::assign<CStats>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTalents>()
    .type("talents"_hs)
    .ctor<&CastCTalents, entt::as_ref_t>()
    .data<&CTalents::acrobatics>("acrobatics"_hs)
    .data<&CTalents::arcana>("arcana"_hs)
    .data<&CTalents::athletics>("athletics"_hs)
    .data<&CTalents::perception>("perception"_hs)
    .data<&CTalents::persuasion>("persuasion"_hs)
    .data<&CTalents::stealth>("stealth"_hs)
    .data<&CTalents::survival>("survival"_hs)
    .data<&CTalents::luck>("luck"_hs)
    .func<&CTalents::print>("print"_hs)
    .func<&CTalents::getInfo>("getInfo"_hs)
    .func<&engine::assign<CTalents>, entt::as_ref_t>("assign"_hs);

  entt::meta<CSkills>()
    .type("skills"_hs)
    .ctor<&CastCSkills, entt::as_ref_t>()
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
    .func<&CSkills::getInfo>("getInfo"_hs)
    .func<&engine::assign<CSkills>, entt::as_ref_t>("assign"_hs);
}
}