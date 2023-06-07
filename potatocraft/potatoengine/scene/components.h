#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "nlohmann/json.hpp"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/buffer.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/texture.h"

namespace potatoengine {

struct UUIDComponent {
    uint64_t uuid;
};

struct Name {
    std::string name{};

    explicit Name(std::string n) : name(std::move(n)) {}
};

struct Tag {
    std::string tag{};

    explicit Tag(std::string t) : tag(std::move(t)) {}
};

struct Deleted {
    bool deleted{false};
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
    std::string filepath{};
    // std::reference_wrapper<Texture> texture; only string?
    // std::reference_wrapper<Shader> shader; only sp? or onCreation?

    Material() = default;
    explicit Material(std::string fp) : filepath(std::move(fp)) {}
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> normals;
};

struct RGBAColor {
    glm::vec4 color;

    explicit RGBAColor(glm::vec4 c) : color(c) {}
};

struct RigidBody {
    float mass;
    float friction;
    float bounciness;

    RigidBody() = default;
    explicit RigidBody(float m, float f, float b) : mass(m), friction(f), bounciness(b) {}
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

    Collider() = default;
    explicit Collider(Type t, glm::vec3 s) : type(t), size(s) {}
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

    Light() = default;
    explicit Light(Type t, glm::vec3 c, float i, float r, float ica, float oca)
        : type(t), color(c), intensity(i), range(r), innerConeAngle(ica), outerConeAngle(oca) {}
};

struct AudioSource {
    // TODO Audio class
    std::string filepath{};
    float volume;
    float pitch;
    bool loop;

    AudioSource() = default;
    explicit AudioSource(std::string fp, float v, float p, bool l) : filepath(std::move(fp)), volume(v), pitch(p), loop(l) {}
};

struct ParticleSystem {
    // TODO define
    std::string emitter{};

    explicit ParticleSystem(std::string e) : emitter(std::move(e)) {}
};

struct Animation {
    // TODO define
    std::string filepath{};

    explicit Animation(std::string fp) : filepath(std::move(fp)) {}
};

struct Text {
    // font class or freetype?
    std::string text{};
    glm::vec4 color;

    Text() = default;
    explicit Text(std::string t, glm::vec4 c) : text(std::move(t)), color(c) {}
};

struct AI {
    // TODO define
    std::string filepath{};

    explicit AI(std::string fp) : filepath(std::move(fp)) {}
};

struct Item {
    // TODO define
    std::string name{};
    std::string description{};
    std::string icon{};
    std::string model{};
    int value;

    Item() = default;
    explicit Item(std::string n, std::string d, std::string i, std::string m, int v)
        : name(std::move(n)), description(std::move(d)), icon(std::move(i)), model(std::move(m)), value(v) {}
};

struct Inventory {
    // TODO define
    std::vector<Item> items{};
};

struct Relationship {
    entt::entity parent{entt::null};
};

struct RendererComponent {
    // TODO define
    std::string filepath{};
};

struct Health {
    int base;
    int current;

    Health() = default;
    explicit Health(int b) : base(b), current(b) {}
};

struct Mana {
    int base;
    int current;

    Mana() = default;
    explicit Mana(int b) : base(b), current(b) {}
};

struct Stamina {
    int base;
    int current;

    Stamina() = default;
    explicit Stamina(int b) : base(b), current(b) {}
};

struct Experience {
    int current;

    Experience() = default;
    explicit Experience(int c) : current(c) {}
};

struct Equipment {
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
};

struct Stats {
    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;

    Stats() = default;
    explicit Stats(int s, int d, int c, int i, int w, int ch)
        : strength(s), dexterity(d), constitution(c), intelligence(i), wisdom(w), charisma(ch) {}
};

struct Talents {
    int acrobatics;
    int arcana;
    int athletics;
    int perception;
    int persuasion;
    int stealth;
    int survival;
    int luck;

    Talents() = default;
    explicit Talents(int a, int ar, int at, int p, int pe, int s, int su, int l)
        : acrobatics(a), arcana(ar), athletics(at), perception(p), persuasion(pe), stealth(s), survival(su), luck(l) {}
};

struct Skills {
    int mining;
    int jewelcrafting;
    int blacksmithing;
    int hunting;
    int fishing;
    int skinning;
    int leatherworking;
    int herbalism;
    int cooking;
    int alchemy;
    int enchanting;
    int harvesting;
    int tailoring;
    int woodworking;
    int woodcutting;
    int farming;
    int quarrying;
    int masonry;

    Skills() = default;
    explicit Skills(int m, int j, int b, int h, int f, int sk, int l, int he, int c, int a, int e, int ha, int t, int w, int wc, int fa, int q, int ma)
        : mining(m), jewelcrafting(j), blacksmithing(b), hunting(h), fishing(f), skinning(sk), leatherworking(l), herbalism(he), cooking(c), alchemy(a), enchanting(e), harvesting(ha), tailoring(t), woodworking(w), woodcutting(wc), farming(fa), quarrying(q), masonry(ma) {}
};

}