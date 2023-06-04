#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/texture.h"

namespace potatoengine {

struct Transform {
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;

    Transform(const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& rot = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
        : pos(pos), rot(rot), scale(scale) {}

    glm::mat4 getTransform() const {
        glm::mat4 rotation = glm::mat4_cast(glm::quat(rot));

        return glm::translate(glm::mat4(1.0f), pos) * rotation * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct Mesh {
    std::unique_ptr<Texture> texture;
    std::string filepath;

    Mesh(const std::string& filepath) : filepath(filepath) {}
};

struct RGBAColor {
    glm::vec4 color;

    RGBAColor(const glm::vec4& color) : color(color) {}
};

struct Tag {
    std::string tag;

    Tag(const std::string& tag) : tag(tag) {}
};

struct ID {
    uint64_t id;

    ID(const uint64_t& id) : id(id) {}
};

struct Name {
    std::string name;

    Name(const std::string& name) : name(name) {}
};

struct RigidBody {
    float mass;
    float friction;
    float bounciness;

    RigidBody(float mass = 1.0f, float friction = 0.0f, float bounciness = 0.0f) : mass(mass), friction(friction), bounciness(bounciness) {}
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

    Collider(Type type = Type::Box, const glm::vec3& size = glm::vec3(1.0f)) : type(type), size(size) {}
};

struct CameraComponent {
    Camera camera;

    CameraComponent(const Camera& camera) : camera(camera) {}
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

    Light(Type type = Type::Point, const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f, float range = 10.0f, float innerConeAngle = 0.0f, float outerConeAngle = 0.0f)
        : type(type), color(color), intensity(intensity), range(range), innerConeAngle(innerConeAngle), outerConeAngle(outerConeAngle) {}
};

struct AudioSource {
    // TODO Audio class
    std::string filepath;
    float volume;
    float pitch;
    bool loop;

    AudioSource(const std::string& filepath, float volume = 1.0f, float pitch = 1.0f, bool loop = false) : filepath(filepath), volume(volume), pitch(pitch), loop(loop) {}
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

    Item(const std::string& name, const std::string& description, const std::string& icon, const std::string& model, int value)
        : name(name), description(description), icon(icon), model(model), value(value) {}
};

struct Inventory {
    // TODO define
    std::vector<Item> items;
};
}