#include "potatoengine/scene/efactory.h"

#include "potatoengine/scene/entity.h"

namespace potatoengine {
Efactory::Efactory(const std::shared_ptr<AssetsManager>& am) : m_assetsManager(am) {}

void Efactory::create(asset::prefab::ID t, Entity& e) {
    auto pfb = m_assetsManager->get<Prefab>(t).get();
    e.addCTags<allCTags>(pfb.getCTags());  // TODO init components on attach
}

void Efactory::destroy(asset::prefab::ID t) {
}

void Efactory::update(asset::prefab::ID t) {
}
}