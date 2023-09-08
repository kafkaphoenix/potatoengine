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

void registerComponents() noexcept {
    using namespace entt::literals;

    entt::meta<UUIDComponent>()
        .type("uuid"_hs)
        .data<&UUIDComponent::uuid>("uuid"_hs);

    entt::meta<Name>()
        .type("name"_hs)
        .data<&Name::name>("name"_hs)
        .func<&AssignValues<Name, std::string>>("assignValues"_hs);

    entt::meta<Tag>()
        .type("tag"_hs)
        .data<&Tag::tag>("tag"_hs)
        .func<&AssignValues<Tag, std::string>>("assignValues"_hs);

    entt::meta<Transform>()
        .type("transform"_hs)
        .data<&Transform::pos>("pos"_hs)
        .data<&Transform::rot>("rot"_hs)
        .data<&Transform::scale>("scale"_hs)
        .func<&Transform::get>("get"_hs)
        .func<&Assign<Transform>>("assign"_hs);

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
        .data<&TextureAtlas::texture>("texture"_hs)
        .data<&TextureAtlas::rows>("rows"_hs)
        .data<&TextureAtlas::index>("index"_hs)
        .func<&Assign<TextureAtlas>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<TextureAtlas, std::shared_ptr<Texture>, int, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<TextureOpts>()
        .type("textureOpts"_hs)
        .data<&TextureOpts::hasTransparency>("hasTransparency"_hs)
        .data<&TextureOpts::useFakeLighting>("useFakeLighting"_hs)
        .func<&Assign<TextureOpts>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<TextureOpts, bool, bool>, entt::as_ref_t>("assignValues"_hs);

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
    
    entt::meta<RGBAColor>()
        .type("rgbaColor"_hs)
        .data<&RGBAColor::color>("color"_hs)
        .func<&AssignValues<RGBAColor, glm::vec4>>("assignValues"_hs);

    entt::meta<RigidBody>()
        .type("rigidBody"_hs)
        .data<&RigidBody::mass>("mass"_hs)
        .data<&RigidBody::friction>("friction"_hs)
        .data<&RigidBody::bounciness>("bounciness"_hs)
        .func<&Assign<RigidBody>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<RigidBody, float, float, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Collider>()
        .type("collider"_hs)
        .data<&Collider::type>("type"_hs)
        .data<&Collider::size>("size"_hs)
        .func<&Assign<Collider>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Collider, Collider::Type, glm::vec3>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<CameraComponent>()
        .type("camera"_hs)
        .data<&CameraComponent::camera>("camera"_hs)
        .func<&Assign<CameraComponent>>("assign"_hs);

    entt::meta<Light>()
        .type("light"_hs)
        .data<&Light::type>("type"_hs)
        .data<&Light::color>("color"_hs)
        .data<&Light::intensity>("intensity"_hs)
        .data<&Light::range>("range"_hs)
        .data<&Light::innerConeAngle>("innerConeAngle"_hs)
        .data<&Light::outerConeAngle>("outerConeAngle"_hs)
        .func<&Assign<Light>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Light, Light::Type, glm::vec3, float, float, float, float>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Audio>()
        .type("audio"_hs)
        .data<&Audio::filepath>("filepath"_hs)
        .data<&Audio::volume>("volume"_hs)
        .data<&Audio::pitch>("pitch"_hs)
        .data<&Audio::loop>("loop"_hs)
        .func<&Assign<Audio>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Audio, std::string, float, float, bool>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<ParticleSystem>()
        .type("particleSystem"_hs)
        .data<&ParticleSystem::emitter>("emitter"_hs)
        .func<&AssignValues<ParticleSystem, std::string>>("assignValues"_hs);

    entt::meta<Animation>()
        .type("animation"_hs)
        .data<&Animation::filepath>("filepath"_hs)
        .func<&AssignValues<Animation, std::string>>("assignValues"_hs);

    entt::meta<Text>()
        .type("text"_hs)
        .data<&Text::text>("text"_hs)
        .data<&Text::color>("color"_hs)
        .func<&Assign<Text>, entt::as_ref_t>("assign"_hs)
        .func<&AssignValues<Text, std::string, glm::vec4>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<AI>()
        .type("ai"_hs)
        .data<&AI::filepath>("filepath"_hs)
        .func<&AssignValues<AI, std::string>>("assignValues"_hs);

    entt::meta<Item>() // TODO probably this is not a component?
        .type("item"_hs)
        .data<&Item::name>("name"_hs)
        .data<&Item::description>("description"_hs)
        .data<&Item::icon>("icon"_hs)
        .data<&Item::model>("model"_hs)
        .data<&Item::value>("value"_hs)
        .func<&AssignValues<Item, std::string, std::string, std::string, std::string, int>, entt::as_ref_t>("assignValues"_hs);

    entt::meta<Inventory>()
        .type("inventory"_hs)
        .data<&Inventory::items>("items"_hs)
        .func<&Assign<Inventory>>("assign"_hs);

    entt::meta<Relationship>()
        .type("relationship"_hs)
        .data<&Relationship::parent>("parent"_hs)
        .func<&Assign<Relationship>>("assign"_hs);

    entt::meta<RendererComponent>()
        .type("renderer"_hs)
        .data<&RendererComponent::filepath>("filepath"_hs)
        .func<&Assign<RendererComponent>>("assign"_hs);

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
        .func<&Assign<Equipment>>("assign"_hs);

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

void printScene(entt::registry& r) noexcept {
    CORE_INFO("Scene entities:");
    r.view<UUIDComponent>().each([&](auto e, auto& uuid) {
        CORE_INFO("\tEntity UUID: {0}", uuid.uuid);

        for (auto&& curr : r.storage()) {
            auto& storage = curr.second;
            if (storage.contains(e)) {
                entt::id_type cid = curr.first;
                entt::type_info ctype = storage.type();
                std::string_view cname = ctype.name().substr(ctype.name().find_last_of(':') + 1);
                uint32_t pos = cname.find("Component");
                CORE_INFO("\t\tComponent {0} ID: {1}", cname.substr(0, pos), cid);

                void* data = storage.get(e);
                if (cname == "Name") {
                    Name* name = static_cast<Name*>(data);
                    CORE_INFO("\t\t\tData: {0}", name->name);
                }

                if (cname == "UUIDComponent") {
                    UUIDComponent* uuid = static_cast<UUIDComponent*>(data);
                    CORE_INFO("\t\t\tData: {0}", uuid->uuid);
                }

                if (cname == "Tag") {
                    Tag* tag = static_cast<Tag*>(data);
                    CORE_INFO("\t\t\tData: {0}", tag->tag);
                }

                if (cname == "Health") {
                    Health* health = static_cast<Health*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", health->base, health->current);
                }

                if (cname == "Material") {
                    Material* material = static_cast<Material*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tambient: {0}\n\t\t\t\t\tdiffuse: {1}\n\t\t\t\t\tspecular: {2}\n\t\t\t\t\tshininess: {3}",
                              glm::to_string(material->ambient), glm::to_string(material->diffuse), glm::to_string(material->specular), material->shininess);
                }

                if (cname == "Stamina") {
                    Stamina* stamina = static_cast<Stamina*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", stamina->base, stamina->current);
                }

                if (cname == "Experience") {
                    Experience* experience = static_cast<Experience*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tcurrent: {0}", experience->current);
                }

                if (cname == "Mana") {
                    Mana* mana = static_cast<Mana*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tbase: {0}\n\t\t\t\t\tcurrent: {1}", mana->base, mana->current);
                }

                if (cname == "Stats") {
                    Stats* stats = static_cast<Stats*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tstrength: {0}\n\t\t\t\t\tdexterity: {1}\n\t\t\t\t\tintelligence: {2}\n\t\t\t\t\tconstitution: {3}\n\t\t\t\t\twisdom: {4}\n\t\t\t\t\tcharisma: {5}",
                              stats->strength, stats->dexterity, stats->intelligence, stats->constitution, stats->wisdom, stats->charisma);
                }

                if (cname == "Talents") {
                    Talents* talents = static_cast<Talents*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tacrobatics: {0}\n\t\t\t\t\tarcana: {1}\n\t\t\t\t\tathletics: {2}\n\t\t\t\t\tperception: {3}\n\t\t\t\t\tpersuasion: {4}\n\t\t\t\t\tstealth: {5}\n\t\t\t\t\tsurvival: {6}\n\t\t\t\t\tluck: {7}",
                              talents->acrobatics, talents->arcana, talents->athletics, talents->perception, talents->persuasion, talents->stealth, talents->survival, talents->luck);
                }

                if (cname == "Skills") {
                    Skills* skills = static_cast<Skills*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\tmining: {0}\n\t\t\t\t\tjewelcrafting: {1}\n\t\t\t\t\tblacksmithing: {2}\n\t\t\t\t\tfishing: {3}\n\t\t\t\t\thunting: {4}\n\t\t\t\t\tskinning: {5}\n\t\t\t\t\tleatherworking: {6}\n\t\t\t\t\therbalism: {7}\n\t\t\t\t\tcooking: {8}\n\t\t\t\t\talchemy: {9}\n\t\t\t\t\tenchanting: {10}\n\t\t\t\t\tharvesting: {11}\n\t\t\t\t\ttailoring: {12}\n\t\t\t\t\twoodworking: {13}\n\t\t\t\t\twoodcutting: {14}\n\t\t\t\t\tfarming: {15}\n\t\t\t\t\tquarrying: {16}\n\t\t\t\t\tmasonry: {17}",
                              skills->mining, skills->jewelcrafting, skills->blacksmithing, skills->fishing, skills->hunting, skills->skinning, skills->leatherworking, skills->herbalism, skills->cooking, skills->alchemy, skills->enchanting, skills->harvesting, skills->tailoring, skills->woodworking, skills->woodcutting, skills->farming, skills->quarrying, skills->masonry);
                }

                if (cname == "TextureAtlas") {
                    TextureAtlas* textureAtlas = static_cast<TextureAtlas*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\ttexture: {0}\n\t\t\t\t\trows: {1}\n\t\t\t\t\tindex: {2}", textureAtlas->texture->getFilepath(), textureAtlas->rows, textureAtlas->index);
                }

                if (cname == "TextureOpts") {
                    TextureOpts* textureOpts = static_cast<TextureOpts*>(data);
                    CORE_INFO("\t\t\tData:\n\t\t\t\t\thasTransparency: {0}\n\t\t\t\t\tuseFakeLighting: {1}", textureOpts->hasTransparency, textureOpts->useFakeLighting);
                }
            }
        }
    });
}
}