#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/buffer.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/texture.h"

namespace potatoengine {

struct UUIDComponent {
    uint64_t uuid;
};

struct Name {
    std::string name;
};

struct Tag {
    std::string tag;
};

struct Transform {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::quat rot = glm::identity<glm::quat>();  // No rotation
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getTransform() const {  // TODO rethink where to do this
        glm::mat4 rotation = glm::mat4_cast(rot);

        return glm::translate(glm::mat4(1.0f), pos) * rotation * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct Material {
    std::string filepath;
    // std::reference_wrapper<Texture> texture; only string?
    // std::reference_wrapper<Shader> shader; only sp? or onCreation?
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> normals;
};

struct RGBAColor {
    glm::vec4 color;
};

struct RigidBody {
    float mass;
    float friction;
    float bounciness;
};

struct Collider {
    enum class Type {
        Box,
        Capsule,
        Mesh,
        Sphere,
    };

    Type type;
    glm::vec3 size;
};

struct CameraComponent {
    Camera camera;
};

struct Light {
    enum class Type {  // https://docs.unity3d.com/Manual/Lighting.html
        Point,
        Spot,
        Directional,
        Area,
    };

    Type type;
    glm::vec3 color;
    float intensity;
    float range;
    float innerConeAngle;
    float outerConeAngle;
};

struct AudioSource {
    // TODO Audio class
    std::string filepath;
    float volume;
    float pitch;
    bool loop;
};

struct ParticleSystem {
    // TODO define
    std::string emitter;
};

struct Animation {
    // TODO define
    std::string filepath;
};

struct Text {
    // font class or freetype?
    std::string text;
    glm::vec4 color;
};

struct AI {
    // TODO define
    std::string filepath;
};

struct Item {
    // TODO define
    std::string name;
    std::string description;
    std::string icon;
    std::string model;
    int value;
};

struct Inventory {
    // TODO define
    std::vector<Item> items;
};

struct Relationship {
    entt::entity parent{entt::null};
};

struct RendererComponent {
    // TODO define
};

struct Health {
    int base;
    int current;
};

struct Mana {
    int base;
    int current;
};

struct Stamina {
    int base;
    int current;
};

struct Experience {
    int current;
};

struct Equipment {
    std::string head;
    std::string neck;
    std::string shoulders;
    std::string chest;
    std::string back;
    std::string hands;
    std::string lfinger;
    std::string rfinger;
    std::string belt;
    std::string legs;
    std::string feet;
};

using allCTags = std::tuple<Transform, Mesh, CameraComponent, Inventory, RendererComponent, Equipment>;
using allComponents = std::tuple<Name, Tag, Health, Mana, Stamina, Experience>;

inline std::unordered_map<std::type_index, std::string> allCTypes = {
    {std::type_index(typeid(UUIDComponent)), "uuid"},
    {std::type_index(typeid(Name)), "name"},
    {std::type_index(typeid(Tag)), "tag"},
    {std::type_index(typeid(Transform)), "transform"},
    {std::type_index(typeid(Material)), "material"},
    {std::type_index(typeid(Mesh)), "mesh"},
    {std::type_index(typeid(RGBAColor)), "rgbacolor"},
    {std::type_index(typeid(RigidBody)), "rigidbody"},
    {std::type_index(typeid(Collider)), "collider"},
    {std::type_index(typeid(CameraComponent)), "camera"},
    {std::type_index(typeid(Light)), "light"},
    {std::type_index(typeid(AudioSource)), "audiosource"},
    {std::type_index(typeid(ParticleSystem)), "particlesystem"},
    {std::type_index(typeid(Animation)), "animation"},
    {std::type_index(typeid(Text)), "text"},
    {std::type_index(typeid(AI)), "ai"},
    {std::type_index(typeid(Item)), "item"},
    {std::type_index(typeid(Inventory)), "inventory"},
    {std::type_index(typeid(Relationship)), "relationship"},
    {std::type_index(typeid(RendererComponent)), "renderer"},
    {std::type_index(typeid(Health)), "health"},
    {std::type_index(typeid(Mana)), "mana"},
    {std::type_index(typeid(Stamina)), "stamina"},
    {std::type_index(typeid(Experience)), "experience"},
    {std::type_index(typeid(Equipment)), "equipment"},
};

}