#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/ai/cAI.h"
#include "scene/components/audio/cAudio.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/camera/cDistanceFromCamera.h"
#include "scene/components/common/cName.h"
#include "scene/components/common/cTag.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/effects/cParticle.h"
#include "scene/components/graphics/cAnimation.h"
#include "scene/components/graphics/cBody.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cMesh.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/graphics/cShape.h"
#include "scene/components/graphics/cText.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/input/cActiveInput.h"
#include "scene/components/input/cInput.h"
#include "scene/components/physics/cCollider.h"
#include "scene/components/physics/cGravity.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/utils/cDeleted.h"
#include "scene/components/utils/cNoise.h"
#include "scene/components/utils/cRelationship.h"
#include "scene/components/world/cBlock.h"
#include "scene/components/world/cChunk.h"
#include "scene/components/world/cChunkManager.h"
#include "scene/components/world/cLight.h"
#include "scene/components/world/cSkybox.h"
#include "scene/components/world/cTime.h"
#include "scene/entity.h"
#include "scene/meta.h"

using namespace entt::literals;

namespace potatoengine {

CUUID& CastCUUID(void* other) { return *static_cast<CUUID*>(other); }

CName& CastCName(void* other) { return *static_cast<CName*>(other); }

CTag& CastCTag(void* other) { return *static_cast<CTag*>(other); }

CShaderProgram& CastCShaderProgram(void* other) {
  return *static_cast<CShaderProgram*>(other);
}

CTransform& CastCTransform(void* other) {
  return *static_cast<CTransform*>(other);
}

CMaterial& CastCMaterial(void* other) {
  return *static_cast<CMaterial*>(other);
}

CTextureAtlas& CastCTextureAtlas(void* other) {
  return *static_cast<CTextureAtlas*>(other);
}

CTexture& CastCTexture(void* other) { return *static_cast<CTexture*>(other); }

CMesh& CastCMesh(void* other) { return *static_cast<CMesh*>(other); }

CBody& CastCBody(void* other) { return *static_cast<CBody*>(other); }

CGravity& CastCGravity(void* other) { return *static_cast<CGravity*>(other); }

CRigidBody& CastCRigidBody(void* other) {
  return *static_cast<CRigidBody*>(other);
}

CCollider& CastCCollider(void* other) {
  return *static_cast<CCollider*>(other);
}

CCamera& CastCCamera(void* other) { return *static_cast<CCamera*>(other); }

CDistanceFromCamera& CastCDistanceFromCamera(void* other) {
  return *static_cast<CDistanceFromCamera*>(other);
}

CActiveCamera& CastCActiveCamera(void* other) {
  return *static_cast<CActiveCamera*>(other);
}

CInput& CastCInput(void* other) { return *static_cast<CInput*>(other); }

CActiveInput& CastCActiveInput(void* other) {
  return *static_cast<CActiveInput*>(other);
}

CSkybox& CastCSkybox(void* other) { return *static_cast<CSkybox*>(other); }

CTime& CastCTime(void* other) { return *static_cast<CTime*>(other); }

CLight& CastCLight(void* other) { return *static_cast<CLight*>(other); }

CAudio& CastCAudio(void* other) { return *static_cast<CAudio*>(other); }

CParticle& CastCParticle(void* other) {
  return *static_cast<CParticle*>(other);
}

CAnimation& CastCAnimation(void* other) {
  return *static_cast<CAnimation*>(other);
}

CText& CastCText(void* other) { return *static_cast<CText*>(other); }

CAI& CastCAI(void* other) { return *static_cast<CAI*>(other); }

CRelationship& CastCRelationship(void* other) {
  return *static_cast<CRelationship*>(other);
}

CShape& CastCShape(void* other) { return *static_cast<CShape*>(other); }

CFBO& CastCFBO(void* other) { return *static_cast<CFBO*>(other); }

CNoise& CastCNoise(void* other) { return *static_cast<CNoise*>(other); }

CChunkManager& CastCChunkManager(void* other) {
  return *static_cast<CChunkManager*>(other);
}

CChunk& CastCChunk(void* other) { return *static_cast<CChunk*>(other); }

CBlock& CastCBlock(void* other) { return *static_cast<CBlock*>(other); }

void RegisterComponents() {
  entt::meta<CUUID>()
    .type("uuid"_hs)
    .ctor<&CastCUUID, entt::as_ref_t>()
    .data<&CUUID::uuid>("uuid"_hs)
    .func<&CUUID::print>("print"_hs)
    .func<&CUUID::getInfo>("getInfo"_hs);

  entt::meta<CName>()
    .type("name"_hs)
    .ctor<&CastCName, entt::as_ref_t>()
    .data<&CName::name>("name"_hs)
    .func<&CName::print>("print"_hs)
    .func<&CName::getInfo>("getInfo"_hs)
    .func<&assign<CName, std::string>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTag>()
    .type("tag"_hs)
    .ctor<&CastCTag, entt::as_ref_t>()
    .data<&CTag::tag>("tag"_hs)
    .func<&CTag::print>("print"_hs)
    .func<&CTag::getInfo>("getInfo"_hs)
    .func<&assign<CTag, std::string>, entt::as_ref_t>("assign"_hs);

  entt::meta<CShaderProgram>()
    .type("shaderProgram"_hs)
    .ctor<&CastCShaderProgram, entt::as_ref_t>()
    .data<&CShaderProgram::name>("name"_hs)
    .data<&CShaderProgram::isVisible>("isVisible"_hs)
    .func<&CShaderProgram::print>("print"_hs)
    .func<&CShaderProgram::getInfo>("getInfo"_hs)
    .func<&assign<CShaderProgram>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTransform>()
    .type("transform"_hs)
    .ctor<&CastCTransform, entt::as_ref_t>()
    .data<&CTransform::position>("position"_hs)
    .data<&CTransform::rotation>("rotation"_hs)
    .data<&CTransform::scale>("scale"_hs)
    .func<&CTransform::print>("print"_hs)
    .func<&CTransform::getInfo>("getInfo"_hs)
    .func<&assign<CTransform>, entt::as_ref_t>("assign"_hs);

  entt::meta<CMaterial>()
    .type("material"_hs)
    .ctor<&CastCMaterial, entt::as_ref_t>()
    .data<&CMaterial::ambient>("ambient"_hs)
    .data<&CMaterial::diffuse>("diffuse"_hs)
    .data<&CMaterial::specular>("specular"_hs)
    .data<&CMaterial::shininess>("shininess"_hs)
    .func<&CMaterial::print>("print"_hs)
    .func<&CMaterial::getInfo>("getInfo"_hs)
    .func<&assign<CMaterial>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTextureAtlas>()
    .type("textureAtlas"_hs)
    .ctor<&CastCTextureAtlas, entt::as_ref_t>()
    .data<&CTextureAtlas::rows>("rows"_hs)
    .data<&CTextureAtlas::index>("index"_hs)
    .func<&CTextureAtlas::print>("print"_hs)
    .func<&CTextureAtlas::getInfo>("getInfo"_hs)
    .func<&assign<CTextureAtlas>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTexture>()
    .type("texture"_hs)
    .ctor<&CastCTexture, entt::as_ref_t>()
    .data<&CTexture::filepaths>("filepaths"_hs)
    .data<&CTexture::textures>("textures"_hs)
    .data<&CTexture::color>("color"_hs)
    .data<&CTexture::blendFactor>("blendFactor"_hs)
    .data<&CTexture::reflectivity>("reflectivity"_hs)
    .data<&CTexture::refractiveIndex>("refractiveIndex"_hs)
    .data<&CTexture::hasTransparency>("hasTransparency"_hs)
    .data<&CTexture::useLighting>("useLighting"_hs)
    .data<&CTexture::useReflection>("useReflection"_hs)
    .data<&CTexture::useRefraction>("useRefraction"_hs)
    .data<&CTexture::_drawMode>("drawMode"_hs)
    .func<&CTexture::print>("print"_hs)
    .func<&CTexture::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CTexture>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CTexture>, entt::as_ref_t>("assign"_hs);

  entt::meta<CMesh>()
    .type("mesh"_hs)
    .ctor<&CastCMesh, entt::as_ref_t>()
    .data<&CMesh::vertices>("vertices"_hs)
    .data<&CMesh::indices>("indices"_hs)
    .data<&CMesh::textures>("textures"_hs)
    .data<&CMesh::vao>("vao"_hs)
    .data<&CMesh::vertexType>("vertexType"_hs)
    .func<&CMesh::print>("print"_hs)
    .func<&CMesh::getInfo>("getInfo"_hs)
    .func<&assign<CMesh>, entt::as_ref_t>("assign"_hs);

  entt::meta<CBody>()
    .type("body"_hs)
    .ctor<&CastCBody, entt::as_ref_t>()
    .data<&CBody::filepath>("filepath"_hs)
    .data<&CBody::meshes>("meshes"_hs)
    .data<&CBody::materials>("materials"_hs)
    .func<&CBody::print>("print"_hs)
    .func<&CBody::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CBody>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CBody, std::string>, entt::as_ref_t>("assign"_hs);

  entt::meta<CGravity>()
    .type("gravity"_hs)
    .ctor<&CastCGravity, entt::as_ref_t>()
    .data<&CGravity::acceleration>("acceleration"_hs)
    .func<&CGravity::print>("print"_hs)
    .func<&CGravity::getInfo>("getInfo"_hs)
    .func<&assign<CGravity>, entt::as_ref_t>("assign"_hs);

  entt::meta<CRigidBody>()
    .type("rigidBody"_hs)
    .ctor<&CastCRigidBody, entt::as_ref_t>()
    .data<&CRigidBody::mass>("mass"_hs)
    .data<&CRigidBody::friction>("friction"_hs)
    .data<&CRigidBody::bounciness>("bounciness"_hs)
    .data<&CRigidBody::isKinematic>("isKinematic"_hs)
    .func<&CRigidBody::print>("print"_hs)
    .func<&CRigidBody::getInfo>("getInfo"_hs)
    .func<&assign<CRigidBody>, entt::as_ref_t>("assign"_hs);

  entt::meta<CCollider>()
    .type("collider"_hs)
    .ctor<&CastCCollider, entt::as_ref_t>()
    .data<&CCollider::_type>("type"_hs)
    .data<&CCollider::size>("size"_hs)
    .func<&CCollider::print>("print"_hs)
    .func<&CCollider::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CCollider>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CCollider>, entt::as_ref_t>("assign"_hs);

  entt::meta<CCamera>()
    .type("camera"_hs)
    .ctor<&CastCCamera, entt::as_ref_t>()
    .data<&CCamera::_type>("type"_hs)
    .data<&CCamera::_aspectRatio>("aspectRatio"_hs)
    .data<&CCamera::_mode>("mode"_hs)
    .data<&CCamera::fov>("fov"_hs)
    .data<&CCamera::zoomFactor>("zoomFactor"_hs)
    .data<&CCamera::zoomMin>("zoomMin"_hs)
    .data<&CCamera::zoomMax>("zoomMax"_hs)
    .data<&CCamera::nearClip>("nearClip"_hs)
    .data<&CCamera::farClip>("farClip"_hs)
    .func<&CCamera::print>("print"_hs)
    .func<&CCamera::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CCamera>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CCamera>, entt::as_ref_t>("assign"_hs);

  entt::meta<CDistanceFromCamera>()
    .type("distanceFromCamera"_hs)
    .ctor<&CastCDistanceFromCamera, entt::as_ref_t>()
    .data<&CDistanceFromCamera::distance>("distance"_hs)
    .func<&CDistanceFromCamera::print>("print"_hs)
    .func<&CDistanceFromCamera::getInfo>("getInfo"_hs)
    .func<&assign<CDistanceFromCamera>, entt::as_ref_t>("assign"_hs);

  entt::meta<CActiveCamera>()
    .type("activeCamera"_hs)
    .ctor<&CastCActiveCamera, entt::as_ref_t>()
    .func<&CActiveCamera::print>("print"_hs)
    .func<&CActiveCamera::getInfo>("getInfo"_hs)
    .func<&assign<CActiveCamera>, entt::as_ref_t>("assign"_hs);

  entt::meta<CInput>()
    .type("input"_hs)
    .ctor<&CastCInput, entt::as_ref_t>()
    .data<&CInput::_mode>("mode"_hs)
    .data<&CInput::mouseSensitivity>("mouseSensitivity"_hs)
    .data<&CInput::translationSpeed>("translationSpeed"_hs)
    .data<&CInput::verticalSpeed>("verticalSpeed"_hs)
    .data<&CInput::rotationSpeed>("rotationSpeed"_hs)
    .func<&CInput::print>("print"_hs)
    .func<&CInput::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CInput>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CInput>, entt::as_ref_t>("assign"_hs);

  entt::meta<CActiveInput>()
    .type("activeInput"_hs)
    .ctor<&CastCActiveInput, entt::as_ref_t>()
    .func<&CActiveInput::print>("print"_hs)
    .func<&CActiveInput::getInfo>("getInfo"_hs)
    .func<&assign<CActiveInput>, entt::as_ref_t>("assign"_hs);

  entt::meta<CSkybox>()
    .type("skybox"_hs)
    .ctor<&CastCSkybox, entt::as_ref_t>()
    .data<&CSkybox::useFog>("useFog"_hs)
    .data<&CSkybox::fogColor>("fogColor"_hs)
    .data<&CSkybox::fogDensity>("fogDensity"_hs)
    .data<&CSkybox::fogGradient>("fogGradient"_hs)
    .data<&CSkybox::rotationSpeed>("rotationSpeed"_hs)
    .func<&CSkybox::print>("print"_hs)
    .func<&CSkybox::getInfo>("getInfo"_hs)
    .func<&assign<CSkybox>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTime>()
    .type("time"_hs)
    .ctor<&CastCTime, entt::as_ref_t>()
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
    .func<&CTime::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CTime>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CTime>, entt::as_ref_t>("assign"_hs);

  entt::meta<CLight>()
    .type("light"_hs)
    .ctor<&CastCLight, entt::as_ref_t>()
    .data<&CLight::_type>("type"_hs)
    .data<&CLight::color>("color"_hs)
    .data<&CLight::intensity>("intensity"_hs)
    .data<&CLight::range>("range"_hs)
    .data<&CLight::innerConeAngle>("innerConeAngle"_hs)
    .data<&CLight::outerConeAngle>("outerConeAngle"_hs)
    .func<&CLight::print>("print"_hs)
    .func<&CLight::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CLight>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CLight>, entt::as_ref_t>("assign"_hs);

  entt::meta<CAudio>()
    .type("audio"_hs)
    .ctor<&CastCAudio, entt::as_ref_t>()
    .data<&CAudio::filepath>("filepath"_hs)
    .data<&CAudio::volume>("volume"_hs)
    .data<&CAudio::pitch>("pitch"_hs)
    .data<&CAudio::loop>("loop"_hs)
    .func<&CAudio::print>("print"_hs)
    .func<&CAudio::getInfo>("getInfo"_hs)
    .func<&assign<CAudio>, entt::as_ref_t>("assign"_hs);

  entt::meta<CParticle>()
    .type("particle"_hs)
    .ctor<&CastCParticle, entt::as_ref_t>()
    .data<&CParticle::emitter>("emitter"_hs)
    .func<&CParticle::print>("print"_hs)
    .func<&CParticle::getInfo>("getInfo"_hs)
    .func<&assign<CParticle>, entt::as_ref_t>("assign"_hs);

  entt::meta<CAnimation>()
    .type("animation"_hs)
    .ctor<&CastCAnimation, entt::as_ref_t>()
    .data<&CAnimation::filepath>("filepath"_hs)
    .func<&CAnimation::print>("print"_hs)
    .func<&CAnimation::getInfo>("getInfo"_hs)
    .func<&assign<CAnimation>, entt::as_ref_t>("assign"_hs);

  entt::meta<CText>()
    .type("text"_hs)
    .ctor<&CastCText, entt::as_ref_t>()
    .data<&CText::text>("text"_hs)
    .data<&CText::color>("color"_hs)
    .func<&CText::print>("print"_hs)
    .func<&CText::getInfo>("getInfo"_hs)
    .func<&assign<CText>, entt::as_ref_t>("assign"_hs);

  entt::meta<CAI>()
    .type("ai"_hs)
    .ctor<&CastCAI, entt::as_ref_t>()
    .data<&CAI::filepath>("filepath"_hs)
    .func<&CAI::print>("print"_hs)
    .func<&CAI::getInfo>("getInfo"_hs)
    .func<&assign<CAI>, entt::as_ref_t>("assign"_hs);

  entt::meta<CRelationship>()
    .type("relationship"_hs)
    .ctor<&CastCRelationship, entt::as_ref_t>()
    .data<&CRelationship::parent>("parent"_hs)
    .func<&CRelationship::print>("print"_hs)
    .func<&CRelationship::getInfo>("getInfo"_hs)
    .func<&assign<CRelationship>, entt::as_ref_t>("assign"_hs);

  entt::meta<CShape>()
    .type("shape"_hs)
    .ctor<&CastCShape, entt::as_ref_t>()
    .data<&CShape::_type>("type"_hs)
    .data<&CShape::size>("size"_hs)
    .data<&CShape::meshes>("meshes"_hs)
    .data<&CShape::repeatTexture>("repeatTexture"_hs)
    .func<&CShape::print>("print"_hs)
    .func<&CShape::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CShape>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CShape>, entt::as_ref_t>("assign"_hs);

  entt::meta<CFBO>()
    .type("fbo"_hs)
    .ctor<&CastCFBO, entt::as_ref_t>()
    .data<&CFBO::fbo>("fbo"_hs)
    .data<&CFBO::_mode>("mode"_hs)
    .data<&CFBO::attachment>("attachment"_hs)
    .data<&CFBO::width>("width"_hs)
    .data<&CFBO::height>("height"_hs)
    .func<&CFBO::print>("print"_hs)
    .func<&CFBO::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CFBO>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CFBO>, entt::as_ref_t>("assign"_hs);

  entt::meta<CNoise>()
    .type("noise"_hs)
    .ctor<&CastCNoise, entt::as_ref_t>()
    .data<&CNoise::_type>("type"_hs)
    .data<&CNoise::seed>("seed"_hs)
    .data<&CNoise::octaves>("octaves"_hs)
    .data<&CNoise::frequency>("frequency"_hs)
    .data<&CNoise::persistence>("persistence"_hs)
    .data<&CNoise::lacunarity>("lacunarity"_hs)
    .data<&CNoise::amplitude>("amplitude"_hs)
    .data<&CNoise::positive>("positive"_hs)
    .func<&CNoise::print>("print"_hs)
    .func<&CNoise::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CNoise>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CNoise>, entt::as_ref_t>("assign"_hs);

  entt::meta<CChunkManager>()
    .type("chunkManager"_hs)
    .ctor<&CastCChunkManager, entt::as_ref_t>()
    .data<&CChunkManager::chunkSize>("chunkSize"_hs)
    .data<&CChunkManager::blockSize>("blockSize"_hs)
    .data<&CChunkManager::width>("width"_hs)
    .data<&CChunkManager::height>("height"_hs)
    .data<&CChunkManager::chunks>("chunks"_hs)
    .data<&CChunkManager::_meshType>("meshType"_hs)
    .data<&CChunkManager::_meshAlgorithm>("meshAlgorithm"_hs)
    .data<&CChunkManager::useBiomes>("useBiomes"_hs)
    .func<&CChunkManager::print>("print"_hs)
    .func<&CChunkManager::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CChunkManager>, entt::as_ref_t>(
      "onComponentAdded"_hs)
    .func<&assign<CChunkManager>, entt::as_ref_t>("assign"_hs);

  entt::meta<CChunk>()
    .type("chunk"_hs)
    .ctor<&CastCChunk, entt::as_ref_t>()
    .data<&CChunk::_biome>("biome"_hs)
    .func<&CChunk::print>("print"_hs)
    .func<&CChunk::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CChunk>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CChunk>, entt::as_ref_t>("assign"_hs);

  entt::meta<CBlock>()
    .type("block"_hs)
    .ctor<&CastCBlock, entt::as_ref_t>()
    .data<&CBlock::_type>("type"_hs)
    .func<&CBlock::print>("print"_hs)
    .func<&CBlock::getInfo>("getInfo"_hs)
    .func<&onComponentAdded<CBlock>, entt::as_ref_t>("onComponentAdded"_hs)
    .func<&assign<CBlock>, entt::as_ref_t>("assign"_hs);
}

void PrintScene(entt::registry& registry) {
  auto entities = registry.view<CUUID>();
  entt::meta_type cType;
  entt::meta_any cData;
  entt::meta_func printFunc;
  std::string_view cName;
  if (entities.empty()) {
    ENGINE_BACKTRACE("===================Entities===================");
    ENGINE_BACKTRACE("No entities in scene");
    ENGINE_BACKTRACE("=============================================");
    return;
  }

  ENGINE_BACKTRACE("===================Entities===================");
  for (auto&& e : entities) {
    ENGINE_BACKTRACE("Entity UUID: {}", entt::to_integral(e));
    for (auto&& [id, storage] : registry.storage()) {
      if (storage.contains(e)) {
        cType = entt::resolve(storage.type());
        cData = cType.construct(storage.value(e));
        printFunc = cType.func("print"_hs);
        if (printFunc) {
          cName = storage.type().name();
          cName = cName.substr(cName.find_last_of(':') + 1);
          ENGINE_BACKTRACE("\t{}", cName);
          printFunc.invoke(cData);
        } else {
          cName = storage.type().name();
          cName = cName.substr(cName.find_last_of(':') + 1);
          ENGINE_ERROR("{} has no print function", cName);
          ENGINE_BACKTRACE("\t{} has no print function", cName);
        }
      }
    }
  }
  ENGINE_BACKTRACE("=============================================");
}
}

// TODO: fix this, they are here because in their components they would add an
// import to entity again and it would be a circular dependency as sceneManager
// already includes entity.h
template <>
void engine::SceneManager::onComponentAdded(Entity& e, CTexture& c) {
  c.setDrawMode();
  const auto& assetsManager = Application::Get().getAssetsManager();

  c.textures.reserve(c.filepaths.size());
  for (std::string_view filepath : c.filepaths) {
    c.textures.emplace_back(assetsManager->get<Texture>(filepath.data()));
  }

  e.update<CTexture>(c);
}

template <>
void engine::SceneManager::onComponentAdded(Entity& e, CChunkManager& c) {
  c.setMeshType();
  c.setMeshAlgorithm();
  e.update<CChunkManager>(c);
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CChunk& c) {
  c.setBiome();
  e.update<CChunk>(c);
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CNoise& c) {
  c.setNoiseType();
  c.setSeed();
  c.setOctaves();
  c.setFrequency();
  c.setPersistence();
  c.setLacunarity();

  e.update<CNoise>(c);
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CBlock& c) {
  c.setBlockType();
  e.update<CBlock>(c);
}

template <> void engine::SceneManager::onComponentAdded(Entity& e, CCamera& c) {
  c.setCameraType();
  c.setAspectRatio();
  c.setMode();
  c.calculateProjection();

  e.update<CCamera>(c);
}
