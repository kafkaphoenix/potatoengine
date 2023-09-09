#pragma once

#include <entt/entt.hpp>
#include <glm/gtx/string_cast.hpp>

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {

template <typename Component>
// cppcheck-suppress unusedFunction
Component& Assign(Entity e) {
    Component& c = e.add<Component>();
    e.onComponentAdded<Component>(c);
    return c;
}

template <typename Component, typename... Args>
// cppcheck-suppress unusedFunction
Component& AssignValues(Entity e, Args... args) {
    return e.add<Component>(std::forward<Args>(args)...);
}

void registerComponents() {
    using namespace entt::literals;

    entt::meta<UUIDComponent>()
        .type("uuid"_hs)
        .data<&UUIDComponent::uuid>("uuid"_hs);

    entt::meta<Name>()
        .type("name"_hs)
        .data<&Name::name>("name"_hs)
        .func<&Assign<Name>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Name, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Tag>()
        .type("tag"_hs)
        .data<&Tag::tag>("tag"_hs)
        .func<&Assign<Tag>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Tag, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Transform>()
        .type("transform"_hs)
        .data<&Transform::position>("position"_hs)
        .data<&Transform::rotation>("rotation"_hs)
        .data<&Transform::scale>("scale"_hs)
        .func<&Transform::calculate>("calculate"_hs)
        .func<&Assign<Transform>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Transform, glm::vec3, glm::vec3, glm::vec3>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Material>()
        .type("material"_hs)
        .data<&Material::ambient>("ambient"_hs)
        .data<&Material::diffuse>("diffuse"_hs)
        .data<&Material::specular>("specular"_hs)
        .data<&Material::shininess>("shininess"_hs)
        .func<&Assign<Material>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Material, glm::vec3, glm::vec3, glm::vec3, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<TextureAtlas>()
        .type("textureAtlas"_hs)
        .data<&TextureAtlas::rows>("rows"_hs)
        .data<&TextureAtlas::index>("index"_hs)
        .func<&Assign<TextureAtlas>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<TextureAtlas, int, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<TextureComponent>()
        .type("texture"_hs)
        .data<&TextureComponent::textures>("textures"_hs)
        .data<&TextureComponent::hasTransparency>("hasTransparency"_hs)
        .data<&TextureComponent::useFakeLighting>("useFakeLighting"_hs)
        .data<&TextureComponent::useBlending>("useBlending"_hs)
        .data<&TextureComponent::blendFactor>("blendFactor"_hs)
        .data<&TextureComponent::useColor>("useColor"_hs)
        .data<&TextureComponent::color>("color"_hs)
        .data<&TextureComponent::useReflection>("useReflection"_hs)
        .data<&TextureComponent::reflectivity>("reflectivity"_hs)
        .data<&TextureComponent::useRefraction>("useRefraction"_hs)
        .data<&TextureComponent::refractiveIndex>("refractiveIndex"_hs)
        .func<&Assign<TextureComponent>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<TextureComponent, std::vector<std::shared_ptr<Texture>>, bool, bool, bool, float, bool, glm::vec4, bool, float, bool, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<ShaderProgramComponent>()
        .type("shaderProgram"_hs)
        .data<&ShaderProgramComponent::shaderProgram>("shaderProgram"_hs)
        .func<&Assign<ShaderProgramComponent>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<ShaderProgramComponent, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Mesh>()
        .type("mesh"_hs)
        .data<&Mesh::vertices>("vertices"_hs)
        .data<&Mesh::indices>("indices"_hs)
        .data<&Mesh::textures>("textures"_hs)
        .data<&Mesh::vao>("vao"_hs)
        .func<&Mesh::setupMesh>("setupMesh"_hs)
        .func<&Assign<Mesh>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Mesh, std::vector<Vertex>, std::vector<uint32_t>, std::vector<std::shared_ptr<Texture>>>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Body>()
        .type("body"_hs)
        .data<&Body::filepath>("filepath"_hs)
        .data<&Body::meshes>("meshes"_hs)
        .data<&Body::materials>("materials"_hs)
        .func<&Assign<Body>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Body, std::string, std::vector<Mesh>, std::vector<Material>>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<RigidBody>()
        .type("rigidBody"_hs)
        .data<&RigidBody::mass>("mass"_hs)
        .data<&RigidBody::friction>("friction"_hs)
        .data<&RigidBody::bounciness>("bounciness"_hs)
        .data<&RigidBody::isKinematic>("isKinematic"_hs)
        .func<&Assign<RigidBody>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<RigidBody, float, float, float, bool>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Collider>()
        .type("collider"_hs)
        .data<&Collider::type>("type"_hs)
        .data<&Collider::size>("size"_hs)
        .func<&Assign<Collider>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Collider, Collider::Type, glm::vec3>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<CameraComponent>()
        .type("camera"_hs)
        .data<&CameraComponent::camera>("camera"_hs)
        .func<&Assign<CameraComponent>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<CameraComponent, Camera>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Skybox>()
        .type("skybox"_hs)
        .data<&Skybox::useFog>("useFog"_hs)
        .data<&Skybox::fogColor>("fogColor"_hs)
        .data<&Skybox::fogDensity>("fogDensity"_hs)
        .data<&Skybox::fogGradient>("fogGradient"_hs)
        .data<&Skybox::skyColor>("skyColor"_hs)
        .data<&Skybox::rotationSpeed>("rotationSpeed"_hs)
        .func<&Assign<Skybox>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Skybox, bool, glm::vec3, float, float, glm::vec3, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<TimeComponent>()
        .type("time"_hs)
        .data<&TimeComponent::seconds>("seconds"_hs)
        .data<&TimeComponent::currentHour>("currentHour"_hs)
        .data<&TimeComponent::currentMinute>("currentMinute"_hs)
        .data<&TimeComponent::currentSecond>("currentSecond"_hs)
        .data<&TimeComponent::startingTime>("startingTime"_hs)
        .data<&TimeComponent::dayLength>("dayLength"_hs)
        .data<&TimeComponent::nightStart>("nightStart"_hs)
        .data<&TimeComponent::dayTransitionStart>("dayTransitionStart"_hs)
        .data<&TimeComponent::dayStart>("dayStart"_hs)
        .data<&TimeComponent::nightTransitionStart>("nightTransitionStart"_hs)
        .data<&TimeComponent::acceleration>("acceleration"_hs)
        .data<&TimeComponent::fps>("fps"_hs)
        .func<&Assign<TimeComponent>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<TimeComponent, float, int, int, int, float, float, float, float, float, float, float, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Light>()
        .type("light"_hs)
        .data<&Light::type>("type"_hs)
        .data<&Light::color>("color"_hs)
        .data<&Light::intensity>("intensity"_hs)
        .data<&Light::range>("range"_hs)
        .data<&Light::innerConeAngle>("innerConeAngle"_hs)
        .data<&Light::outerConeAngle>("outerConeAngle"_hs)
        .func<&Assign<Light>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Light, Light::Type, glm::vec4, float, float, float, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Audio>()
        .type("audio"_hs)
        .data<&Audio::filepath>("filepath"_hs)
        .data<&Audio::volume>("volume"_hs)
        .data<&Audio::pitch>("pitch"_hs)
        .data<&Audio::loop>("loop"_hs)
        .func<&Assign<Audio>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Audio, std::string, float, float, bool>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<ParticleComponent>()
        .type("particleComponent"_hs)
        .data<&ParticleComponent::emitter>("emitter"_hs)
        .func<&Assign<ParticleComponent>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<ParticleComponent, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Animation>()
        .type("animation"_hs)
        .data<&Animation::filepath>("filepath"_hs)
        .func<&Assign<Animation>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Animation, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Text>()
        .type("text"_hs)
        .data<&Text::text>("text"_hs)
        .data<&Text::color>("color"_hs)
        .func<&Assign<Text>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Text, std::string, glm::vec4>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<AI>()
        .type("ai"_hs)
        .data<&AI::filepath>("filepath"_hs)
        .func<&Assign<AI>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<AI, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Item>()  // TODO probably this is not a component?
        .type("item"_hs)
        .data<&Item::name>("name"_hs)
        .data<&Item::description>("description"_hs)
        .data<&Item::icon>("icon"_hs)
        .data<&Item::model>("model"_hs)
        .data<&Item::value>("value"_hs)
        .func<&Assign<Item>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Item, std::string, std::string, std::string, std::string, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Inventory>()
        .type("inventory"_hs)
        .data<&Inventory::items>("items"_hs)
        .func<&Assign<Inventory>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Inventory, std::vector<Item>>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Relationship>()
        .type("relationship"_hs)
        .data<&Relationship::parent>("parent"_hs)
        .func<&Assign<Relationship>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Relationship, Entity>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Health>()
        .type("health"_hs)
        .data<&Health::base>("base"_hs)
        .data<&Health::current>("current"_hs)
        .func<&Assign<Health>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Health, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Mana>()
        .type("mana"_hs)
        .data<&Mana::base>("base"_hs)
        .data<&Mana::current>("current"_hs)
        .func<&Assign<Mana>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Mana, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Stamina>()
        .type("stamina"_hs)
        .data<&Stamina::base>("base"_hs)
        .data<&Stamina::current>("current"_hs)
        .func<&Assign<Stamina>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Stamina, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Experience>()
        .type("experience"_hs)
        .data<&Experience::current>("current"_hs)
        .func<&Assign<Experience>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Experience, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Equipment>()
        .type("equipment"_hs)
        .data<&Equipment::head>("head"_hs)
        .data<&Equipment::neck>("neck"_hs)
        .data<&Equipment::shoulders>("shoulders"_hs)
        .data<&Equipment::chest>("chest"_hs)
        .data<&Equipment::back>("back"_hs)
        .data<&Equipment::hands>("hands"_hs)
        .data<&Equipment::lfinger>("lfinger"_hs)
        .data<&Equipment::rfinger>("rfinger"_hs)
        .data<&Equipment::belt>("belt"_hs)
        .data<&Equipment::legs>("legs"_hs)
        .data<&Equipment::feet>("feet"_hs)
        .func<&Assign<Equipment>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Equipment, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Stats>()
        .type("stats"_hs)
        .data<&Stats::strength>("strength"_hs)
        .data<&Stats::dexterity>("dexterity"_hs)
        .data<&Stats::constitution>("constitution"_hs)
        .data<&Stats::intelligence>("intelligence"_hs)
        .data<&Stats::wisdom>("wisdom"_hs)
        .data<&Stats::charisma>("charisma"_hs)
        .func<&Assign<Stats>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Stats, int, int, int, int, int, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Talents>()
        .type("talents"_hs)
        .data<&Talents::acrobatics>("acrobatics"_hs)
        .data<&Talents::arcana>("arcana"_hs)
        .data<&Talents::athletics>("athletics"_hs)
        .data<&Talents::perception>("perception"_hs)
        .data<&Talents::persuasion>("persuasion"_hs)
        .data<&Talents::stealth>("stealth"_hs)
        .data<&Talents::survival>("survival"_hs)
        .data<&Talents::luck>("luck"_hs)
        .func<&Assign<Talents>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Talents, int, int, int, int, int, int, int, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Skills>()
        .type("skills"_hs)
        .data<&Skills::mining>("mining"_hs)
        .data<&Skills::jewelcrafting>("jewelcrafting"_hs)
        .data<&Skills::blacksmithing>("blacksmithing"_hs)
        .data<&Skills::fishing>("fishing"_hs)
        .data<&Skills::hunting>("hunting"_hs)
        .data<&Skills::skinning>("skinning"_hs)
        .data<&Skills::leatherworking>("leatherworking"_hs)
        .data<&Skills::herbalism>("herbalism"_hs)
        .data<&Skills::cooking>("cooking"_hs)
        .data<&Skills::alchemy>("alchemy"_hs)
        .data<&Skills::enchanting>("enchanting"_hs)
        .data<&Skills::harvesting>("harvesting"_hs)
        .data<&Skills::tailoring>("tailoring"_hs)
        .data<&Skills::woodworking>("woodworking"_hs)
        .data<&Skills::woodcutting>("woodcutting"_hs)
        .data<&Skills::farming>("farming"_hs)
        .data<&Skills::quarrying>("quarrying"_hs)
        .data<&Skills::masonry>("masonry"_hs)
        .func<&Assign<Skills>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Skills, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int>, entt::as_ref_t>("assignValues"_hs);
}

void printScene(entt::registry& r) {
    CORE_INFO("Scene entities:");
    r.view<UUIDComponent>().each([&](entt::entity e, const UUIDComponent& cUUID) {
        CORE_INFO("\tEntity UUID: {0}", cUUID.uuid);

        for (auto&& curr : r.storage()) {
            auto& storage = curr.second;
            if (storage.contains(e)) {
                entt::id_type cID = curr.first;
                entt::type_info cType = storage.type();
                std::string_view cName = cType.name().substr(cType.name().find_last_of(':') + 1);
                uint32_t cIndex = cName.find("Component");
                CORE_INFO("\t\tComponent {0} ID: {1}", cName.substr(0, cIndex), cID);

                void* data = storage.get(e);
                if (cName == "Name") {
                    Name* _cName = static_cast<Name*>(data);
                    CORE_INFO("\t\t\tname: {0}", _cName->name);
                }

                if (cName == "UUIDComponent") {
                    CORE_INFO("\t\t\tuuid: {0}", cUUID.uuid);
                }

                if (cName == "Tag") {
                    Tag* cTag = static_cast<Tag*>(data);
                    CORE_INFO("\t\t\ttag: {0}", cTag->tag);
                }

                if (cName == "Transform") {
                    Transform* cTransform = static_cast<Transform*>(data);
                    CORE_INFO("\t\t\tposition: {0}\n\t\t\t\t\trotation: {1}\n\t\t\t\t\tscale: {2}", glm::to_string(cTransform->position), glm::to_string(cTransform->rotation), glm::to_string(cTransform->scale));
                }

                if (cName == "Health") {
                    Health* cHealth = static_cast<Health*>(data);
                    CORE_INFO("\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", cHealth->base, cHealth->current);
                }

                if (cName == "Material") {
                    Material* cMaterial = static_cast<Material*>(data);
                    CORE_INFO("\t\t\tambient: {0}\n\t\t\t\t\tdiffuse: {1}\n\t\t\t\t\tspecular: {2}\n\t\t\t\t\tshininess: {3}",
                              glm::to_string(cMaterial->ambient), glm::to_string(cMaterial->diffuse), glm::to_string(cMaterial->specular), cMaterial->shininess);
                }

                if (cName == "Stamina") {
                    Stamina* cStamina = static_cast<Stamina*>(data);
                    CORE_INFO("\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", cStamina->base, cStamina->current);
                }

                if (cName == "Experience") {
                    Experience* cExperience = static_cast<Experience*>(data);
                    CORE_INFO("\t\t\tcurrent: {0}", cExperience->current);
                }

                if (cName == "Mana") {
                    Mana* cMana = static_cast<Mana*>(data);
                    CORE_INFO("\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", cMana->base, cMana->current);
                }

                if (cName == "Stats") {
                    Stats* cStats = static_cast<Stats*>(data);
                    CORE_INFO("\t\t\tstrength: {0}\n\t\t\t\t\tdexterity: {1}\n\t\t\t\t\tintelligence: {2}\n\t\t\t\t\tconstitution: {3}\n\t\t\t\t\twisdom: {4}\n\t\t\t\t\tcharisma: {5}",
                              cStats->strength, cStats->dexterity, cStats->intelligence, cStats->constitution, cStats->wisdom, cStats->charisma);
                }

                if (cName == "Talents") {
                    Talents* cTalents = static_cast<Talents*>(data);
                    CORE_INFO("\t\t\tacrobatics: {0}\n\t\t\t\t\tarcana: {1}\n\t\t\t\t\tathletics: {2}\n\t\t\t\t\tperception: {3}\n\t\t\t\t\tpersuasion: {4}\n\t\t\t\t\tstealth: {5}\n\t\t\t\t\tsurvival: {6}\n\t\t\t\t\tluck: {7}",
                              cTalents->acrobatics, cTalents->arcana, cTalents->athletics, cTalents->perception, cTalents->persuasion, cTalents->stealth, cTalents->survival, cTalents->luck);
                }

                if (cName == "Skills") {
                    Skills* cSkills = static_cast<Skills*>(data);
                    CORE_INFO("\t\t\tmining: {0}\n\t\t\t\t\tjewelcrafting: {1}\n\t\t\t\t\tblacksmithing: {2}\n\t\t\t\t\tfishing: {3}\n\t\t\t\t\thunting: {4}\n\t\t\t\t\tskinning: {5}\n\t\t\t\t\tleatherworking: {6}\n\t\t\t\t\therbalism: {7}\n\t\t\t\t\tcooking: {8}\n\t\t\t\t\talchemy: {9}\n\t\t\t\t\tenchanting: {10}\n\t\t\t\t\tharvesting: {11}\n\t\t\t\t\ttailoring: {12}\n\t\t\t\t\twoodworking: {13}\n\t\t\t\t\twoodcutting: {14}\n\t\t\t\t\tfarming: {15}\n\t\t\t\t\tquarrying: {16}\n\t\t\t\t\tmasonry: {17}",
                              cSkills->mining, cSkills->jewelcrafting, cSkills->blacksmithing, cSkills->fishing, cSkills->hunting, cSkills->skinning, cSkills->leatherworking, cSkills->herbalism, cSkills->cooking, cSkills->alchemy, cSkills->enchanting, cSkills->harvesting, cSkills->tailoring, cSkills->woodworking, cSkills->woodcutting, cSkills->farming, cSkills->quarrying, cSkills->masonry);
                }

                if (cName == "TextureComponent") {
                    TextureComponent* cTexture = static_cast<TextureComponent*>(data);
                    std::string texturePaths;
                    for (const auto& texture : cTexture->textures) {
                        texturePaths += "\n\t\t\t\t\ttexture: " + std::string(texture->getFilepath());
                    }

                    CORE_INFO("\t\t\ttextures: {0}\n\t\t\t\t\thasTransparency: {1}\n\t\t\t\t\tuseFakeLighting: {2}\n\t\t\t\t\tuseBlending: {3}\n\t\t\t\t\tblendFactor: {4}\n\t\t\t\t\tuseColor: {5}\n\t\t\t\t\tcolor: {6}\n\t\t\t\t\tuseReflection: {7}\n\t\t\t\t\treflectivity: {8}\n\t\t\t\t\tuseRefraction: {9}\n\t\t\t\t\trefractiveIndex: {10}{11}",
                              cTexture->textures.size(), cTexture->hasTransparency, cTexture->useFakeLighting, cTexture->useBlending, cTexture->blendFactor, cTexture->useColor, glm::to_string(cTexture->color), cTexture->useReflection, cTexture->reflectivity, cTexture->useRefraction, cTexture->refractiveIndex, texturePaths);
                }

                if (cName == "ShaderProgramComponent") {
                    ShaderProgramComponent* cShaderProgram = static_cast<ShaderProgramComponent*>(data);
                    CORE_INFO("\t\t\tshaderProgram: {0}", cShaderProgram->shaderProgram);
                }

                if (cName == "Body") {
                    Body* cBody = static_cast<Body*>(data);
                    CORE_INFO("\t\t\tfilepath: {0}", cBody->filepath);
                }

                if (cName == "RigidBody") {
                    RigidBody* cRigidBody = static_cast<RigidBody*>(data);
                    CORE_INFO("\t\t\tmass: {0}\n\t\t\t\t\tfriction: {1}\n\t\t\t\t\tbounciness: {2}\n\t\t\t\t\tisKinematic: {3}", cRigidBody->mass, cRigidBody->friction, cRigidBody->bounciness, cRigidBody->isKinematic);
                }

                if (cName == "TextureAtlas") {
                    TextureAtlas* cTextureAtlas = static_cast<TextureAtlas*>(data);
                    CORE_INFO("\t\t\trows: {0}\n\t\t\t\t\tindex: {1}", cTextureAtlas->rows, cTextureAtlas->index);
                }

                if (cName == "Skybox") {
                    Skybox* cSkybox = static_cast<Skybox*>(data);
                    CORE_INFO("\t\t\tuseFog: {0}\n\t\t\t\t\tfogDensity: {1}\n\t\t\t\t\tfogGradient: {2}\n\t\t\t\t\tskyColor: {3}\n\t\t\t\t\trotationSpeed: {4}",
                              cSkybox->useFog, cSkybox->fogDensity, cSkybox->fogGradient, glm::to_string(cSkybox->skyColor), cSkybox->rotationSpeed);
                }

                if (cName == "Light") {
                    Light* cLight = static_cast<Light*>(data);
                    CORE_INFO("\t\t\ttype: {0}\n\t\t\t\t\tcolor: {1}\n\t\t\t\t\tintensity: {2}\n\t\t\t\t\trange: {3}\n\t\t\t\t\tinnerConeAngle: {4}\n\t\t\t\t\touterConeAngle: {5}",
                              cLight->to_string(cLight->type), glm::to_string(cLight->color), cLight->intensity, cLight->range, cLight->innerConeAngle, cLight->outerConeAngle);
                }

                if (cName == "TimeComponent") {
                    TimeComponent* cTime = static_cast<TimeComponent*>(data);
                    CORE_INFO("\t\t\tseconds: {0}\n\t\t\t\t\tcurrentHour: {1}\n\t\t\t\t\tcurrentMinute: {2}\n\t\t\t\t\tcurrentSecond: {3}\n\t\t\t\t\tstartingTime: {4}\n\t\t\t\t\tdayLength: {5}\n\t\t\t\t\tnightStart: {6}\n\t\t\t\t\tdayTransitionStart: {7}\n\t\t\t\t\tdayStart: {8}\n\t\t\t\t\tnightTransitionStart: {9}\n\t\t\t\t\tacceleration: {10}\n\t\t\t\t\tfps: {11}",
                              cTime->seconds, cTime->currentHour, cTime->currentMinute, cTime->currentSecond, cTime->startingTime, cTime->dayLength, cTime->nightStart, cTime->dayTransitionStart, cTime->dayStart, cTime->nightTransitionStart, cTime->acceleration, cTime->fps);
                }
            }
        }
    });
}
}