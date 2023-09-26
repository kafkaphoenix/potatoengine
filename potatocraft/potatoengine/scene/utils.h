#pragma once

#include <entt/entt.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/scene/components/ai/cAI.h"
#include "potatoengine/scene/components/audio/cAudio.h"
#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cTag.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/effects/cParticle.h"
#include "potatoengine/scene/components/graphics/cAnimation.h"
#include "potatoengine/scene/components/graphics/cBody.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/scene/components/graphics/cShaderProgram.h"
#include "potatoengine/scene/components/graphics/cText.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/graphics/cTextureAtlas.h"
#include "potatoengine/scene/components/physics/cCollider.h"
#include "potatoengine/scene/components/physics/cRigidBody.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/utils/cDeleted.h"
#include "potatoengine/scene/components/utils/cRelationship.h"
#include "potatoengine/scene/components/world/cLight.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/world/cTime.h"
#include "potatoengine/scene/entity.h"
#include "potatoengine/scene/meta.h"

namespace potatoengine {

CUUID &castCUUID(void *other) {
    return *static_cast<CUUID *>(other);
}

CName &castCName(void *other) {
    return *static_cast<CName *>(other);
}

CTag &castCTag(void *other) {
    return *static_cast<CTag *>(other);
}

CShaderProgram &castCShaderProgram(void *other) {
    return *static_cast<CShaderProgram *>(other);
}

CTransform &castCTransform(void *other) {
    return *static_cast<CTransform *>(other);
}

CMaterial &castCMaterial(void *other) {
    return *static_cast<CMaterial *>(other);
}

CTextureAtlas &castCTextureAtlas(void *other) {
    return *static_cast<CTextureAtlas *>(other);
}

CTexture &castCTexture(void *other) {
    return *static_cast<CTexture *>(other);
}

CMesh &castCMesh(void *other) {
    return *static_cast<CMesh *>(other);
}

CBody &castCBody(void *other) {
    return *static_cast<CBody *>(other);
}

CRigidBody &castCRigidBody(void *other) {
    return *static_cast<CRigidBody *>(other);
}

CCollider &castCCollider(void *other) {
    return *static_cast<CCollider *>(other);
}

CCamera &castCCamera(void *other) {
    return *static_cast<CCamera *>(other);
}

CSkybox &castCSkybox(void *other) {
    return *static_cast<CSkybox *>(other);
}

CTime &castCTime(void *other) {
    return *static_cast<CTime *>(other);
}

CLight &castCLight(void *other) {
    return *static_cast<CLight *>(other);
}

CAudio &castCAudio(void *other) {
    return *static_cast<CAudio *>(other);
}

CParticle &castCParticle(void *other) {
    return *static_cast<CParticle *>(other);
}

CAnimation &castCAnimation(void *other) {
    return *static_cast<CAnimation *>(other);
}

CText &castCText(void *other) {
    return *static_cast<CText *>(other);
}

CAI &castCAI(void *other) {
    return *static_cast<CAI *>(other);
}

CRelationship &castCRelationship(void *other) {
    return *static_cast<CRelationship *>(other);
}

void registerComponents() {
    using namespace entt::literals;

    entt::meta<CUUID>()
        .type("uuid"_hs)
        .ctor<&castCUUID, entt::as_ref_t>()
        .data<&CUUID::uuid>("uuid"_hs)
        .func<&CUUID::print>("print"_hs);

    entt::meta<CName>()
        .type("name"_hs)
        .ctor<&castCName, entt::as_ref_t>()
        .data<&CName::name>("name"_hs)
        .func<&CName::print>("print"_hs)
        .func<&assign<CName, std::string>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTag>()
        .type("tag"_hs)
        .ctor<&castCTag, entt::as_ref_t>()
        .data<&CTag::tag>("tag"_hs)
        .func<&CTag::print>("print"_hs)
        .func<&assign<CTag, std::string>, entt::as_ref_t>("assign"_hs);

    entt::meta<CShaderProgram>()
        .type("shaderProgram"_hs)
        .ctor<&castCShaderProgram, entt::as_ref_t>()
        .data<&CShaderProgram::shaderProgram>("shaderProgram"_hs)
        .func<&CShaderProgram::print>("print"_hs)
        .func<&assign<CShaderProgram, std::string>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTransform>()
        .type("transform"_hs)
        .ctor<&castCTransform, entt::as_ref_t>()
        .data<&CTransform::position>("position"_hs)
        .data<&CTransform::rotation>("rotation"_hs)
        .data<&CTransform::scale>("scale"_hs)
        .func<&CTransform::calculate>("calculate"_hs)
        .func<&CTransform::print>("print"_hs)
        .func<&assign<CTransform>, entt::as_ref_t>("assign"_hs);

    entt::meta<CMaterial>()
        .type("material"_hs)
        .ctor<&castCMaterial, entt::as_ref_t>()
        .data<&CMaterial::ambient>("ambient"_hs)
        .data<&CMaterial::diffuse>("diffuse"_hs)
        .data<&CMaterial::specular>("specular"_hs)
        .data<&CMaterial::shininess>("shininess"_hs)
        .func<&CMaterial::print>("print"_hs)
        .func<&assign<CMaterial>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTextureAtlas>()
        .type("textureAtlas"_hs)
        .ctor<&castCTextureAtlas, entt::as_ref_t>()
        .data<&CTextureAtlas::rows>("rows"_hs)
        .data<&CTextureAtlas::index>("index"_hs)
        .func<&CTextureAtlas::print>("print"_hs)
        .func<&assign<CTextureAtlas>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTexture>()
        .type("texture"_hs)
        .ctor<&castCTexture, entt::as_ref_t>()
        .data<&CTexture::filepaths>("filepaths"_hs)
        .data<&CTexture::textures>("textures"_hs)
        .data<&CTexture::hasTransparency>("hasTransparency"_hs)
        .data<&CTexture::useFakeLighting>("useFakeLighting"_hs)
        .data<&CTexture::useBlending>("useBlending"_hs)
        .data<&CTexture::blendFactor>("blendFactor"_hs)
        .data<&CTexture::useColor>("useColor"_hs)
        .data<&CTexture::color>("color"_hs)
        .data<&CTexture::useReflection>("useReflection"_hs)
        .data<&CTexture::reflectivity>("reflectivity"_hs)
        .data<&CTexture::useRefraction>("useRefraction"_hs)
        .data<&CTexture::refractiveIndex>("refractiveIndex"_hs)
        .func<&CTexture::print>("print"_hs)
        .func<&onComponentAdded<CTexture>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CTexture>, entt::as_ref_t>("assign"_hs);

    entt::meta<CMesh>()
        .type("mesh"_hs)
        .ctor<&castCMesh, entt::as_ref_t>()
        .data<&CMesh::vertices>("vertices"_hs)
        .data<&CMesh::indices>("indices"_hs)
        .data<&CMesh::textures>("textures"_hs)
        .data<&CMesh::vao>("vao"_hs)
        .func<&CMesh::setupMesh>("setupMesh"_hs)
        .func<&CMesh::print>("print"_hs)
        .func<&assign<CMesh>, entt::as_ref_t>("assign"_hs);

    entt::meta<CBody>()
        .type("body"_hs)
        .ctor<&castCBody, entt::as_ref_t>()
        .data<&CBody::filepath>("filepath"_hs)
        .data<&CBody::meshes>("meshes"_hs)
        .data<&CBody::materials>("materials"_hs)
        .func<&CBody::print>("print"_hs)
        .func<&onComponentAdded<CBody>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CBody, std::string>, entt::as_ref_t>("assign"_hs);

    entt::meta<CRigidBody>()
        .type("rigidBody"_hs)
        .ctor<&castCRigidBody, entt::as_ref_t>()
        .data<&CRigidBody::mass>("mass"_hs)
        .data<&CRigidBody::friction>("friction"_hs)
        .data<&CRigidBody::bounciness>("bounciness"_hs)
        .data<&CRigidBody::isKinematic>("isKinematic"_hs)
        .func<&CRigidBody::print>("print"_hs)
        .func<&assign<CRigidBody>, entt::as_ref_t>("assign"_hs);

    entt::meta<CCollider>()
        .type("collider"_hs)
        .ctor<&castCCollider, entt::as_ref_t>()
        .data<&CCollider::_type>("type"_hs)
        .data<&CCollider::size>("size"_hs)
        .func<&CCollider::print>("print"_hs)
        .func<&onComponentAdded<CCollider>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CCollider>, entt::as_ref_t>("assign"_hs);

    entt::meta<CCamera>()
        .type("camera"_hs)
        .ctor<&castCCamera, entt::as_ref_t>()
        .data<&CCamera::camera>("camera"_hs)
        .func<&CCamera::print>("print"_hs)
        .func<&onComponentAdded<CCamera>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CCamera>, entt::as_ref_t>("assign"_hs);

    entt::meta<CSkybox>()
        .type("skybox"_hs)
        .ctor<&castCSkybox, entt::as_ref_t>()
        .data<&CSkybox::useFog>("useFog"_hs)
        .data<&CSkybox::fogColor>("fogColor"_hs)
        .data<&CSkybox::fogDensity>("fogDensity"_hs)
        .data<&CSkybox::fogGradient>("fogGradient"_hs)
        .data<&CSkybox::rotationSpeed>("rotationSpeed"_hs)
        .func<&CSkybox::print>("print"_hs)
        .func<&assign<CSkybox>, entt::as_ref_t>("assign"_hs);

    entt::meta<CTime>()
        .type("time"_hs)
        .ctor<&castCTime, entt::as_ref_t>()
        .data<&CTime::seconds>("seconds"_hs)
        .data<&CTime::currentHour>("currentHour"_hs)
        .data<&CTime::currentMinute>("currentMinute"_hs)
        .data<&CTime::currentSecond>("currentSecond"_hs)
        .data<&CTime::dayLength>("dayLength"_hs)
        .data<&CTime::startingTime>("startingTime"_hs)
        .data<&CTime::nightStart>("nightStart"_hs)
        .data<&CTime::dayTransitionStart>("dayTransitionStart"_hs)
        .data<&CTime::dayStart>("dayStart"_hs)
        .data<&CTime::nightTransitionStart>("nightTransitionStart"_hs)
        .data<&CTime::acceleration>("acceleration"_hs)
        .data<&CTime::fps>("fps"_hs)
        .func<&CTime::print>("print"_hs)
        .func<&onComponentAdded<CTime>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CTime>, entt::as_ref_t>("assign"_hs);

    entt::meta<CLight>()
        .type("light"_hs)
        .ctor<&castCLight, entt::as_ref_t>()
        .data<&CLight::_type>("type"_hs)
        .data<&CLight::color>("color"_hs)
        .data<&CLight::intensity>("intensity"_hs)
        .data<&CLight::range>("range"_hs)
        .data<&CLight::innerConeAngle>("innerConeAngle"_hs)
        .data<&CLight::outerConeAngle>("outerConeAngle"_hs)
        .func<&CLight::print>("print"_hs)
        .func<&onComponentAdded<CLight>, entt::as_ref_t>("onComponentAdded"_hs)
        .func<&assign<CLight>, entt::as_ref_t>("assign"_hs);

    entt::meta<CAudio>()
        .type("audio"_hs)
        .ctor<&castCAudio, entt::as_ref_t>()
        .data<&CAudio::filepath>("filepath"_hs)
        .data<&CAudio::volume>("volume"_hs)
        .data<&CAudio::pitch>("pitch"_hs)
        .data<&CAudio::loop>("loop"_hs)
        .func<&CAudio::print>("print"_hs)
        .func<&assign<CAudio>, entt::as_ref_t>("assign"_hs);

    entt::meta<CParticle>()
        .type("particle"_hs)
        .ctor<&castCParticle, entt::as_ref_t>()
        .data<&CParticle::emitter>("emitter"_hs)
        .func<&CParticle::print>("print"_hs)
        .func<&assign<CParticle>, entt::as_ref_t>("assign"_hs);

    entt::meta<CAnimation>()
        .type("animation"_hs)
        .ctor<&castCAnimation, entt::as_ref_t>()
        .data<&CAnimation::filepath>("filepath"_hs)
        .func<&CAnimation::print>("print"_hs)
        .func<&assign<CAnimation>, entt::as_ref_t>("assign"_hs);

    entt::meta<CText>()
        .type("text"_hs)
        .ctor<&castCText, entt::as_ref_t>()
        .data<&CText::text>("text"_hs)
        .data<&CText::color>("color"_hs)
        .func<&CText::print>("print"_hs)
        .func<&assign<CText>, entt::as_ref_t>("assign"_hs);

    entt::meta<CAI>()
        .type("ai"_hs)
        .ctor<&castCAI, entt::as_ref_t>()
        .data<&CAI::filepath>("filepath"_hs)
        .func<&CAI::print>("print"_hs)
        .func<&assign<CAI>, entt::as_ref_t>("assign"_hs);

    entt::meta<CRelationship>()
        .type("relationship"_hs)
        .ctor<&castCRelationship, entt::as_ref_t>()
        .data<&CRelationship::parent>("parent"_hs)
        .func<&CRelationship::print>("print"_hs)
        .func<&assign<CRelationship>, entt::as_ref_t>("assign"_hs);
}

void printScene(entt::registry &r) {
    using namespace entt::literals;
    CORE_INFO("Scene entities:");
    r.view<CUUID>().each([&](entt::entity e, const CUUID &cUUID) {
        CORE_INFO("Entity UUID: {0}", entt::to_integral(e));
        for (auto&& curr : r.storage()) {
            if (auto& storage = curr.second; storage.contains(e)) {
                entt::meta_type cType = entt::resolve(storage.type());
                entt::meta_any cData = cType.construct(storage.value(e));
                entt::meta_func printFunc = cType.func("print"_hs);
                if (printFunc) {
                    std::string_view cName = storage.type().name();
                    cName = cName.substr(cName.find_last_of(':') + 1);
                    CORE_INFO("\t{0}", cName);
                    printFunc.invoke(cData);
                } else {
                    throw std::runtime_error("Component does not have a print function");
                }
            }
        }
    });
}
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CTexture &c) {
    const auto &manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    std::vector<std::shared_ptr<Texture>> textures;
    for (std::string_view filepath : c.filepaths) {
        textures.emplace_back(manager->get<Texture>(filepath.data()));
    }

    c.textures = std::move(textures);
    e.update<CTexture>(c);
}