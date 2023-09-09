#pragma once

#include <entt/entt.hpp>
#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "nlohmann/json.hpp"
#include "potatoengine/assets/texture.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/buffer.h"
#include "potatoengine/renderer/camera/camera.h"
#include "potatoengine/renderer/shaderProgram.h"
#include "potatoengine/renderer/vao.h"

namespace potatoengine {

struct UUIDComponent {
    uint64_t uuid{};
};

struct Name {
    std::string name{};

    Name() = default;
    explicit Name(std::string n) : name(std::move(n)) {}
};

struct Tag {
    std::string tag{};

    Tag() = default;
    explicit Tag(std::string t) : tag(std::move(t)) {}
};

struct Deleted {
    bool deleted{};
};

struct Transform {
    glm::vec3 position{};
    glm::quat rotation{};
    glm::vec3 scale{glm::vec3{1.f}};

    Transform() = default;
    explicit Transform(glm::vec3 p, glm::quat r, glm::vec3 s) : position(std::move(p)), rotation(std::move(r)), scale(std::move(s)) {}

    glm::mat4 calculate() const {
        // T * R * S
        return glm::scale(glm::translate(glm::mat4(1.f), position) * glm::mat4_cast(rotation), scale);
    }

    void rotate(float angle, const glm::vec3& axis) {
        rotation = glm::angleAxis(glm::radians(angle), axis) * rotation;
    }

    void rotate(const glm::quat& q) {
        rotation = q * rotation;
    }
};

struct Material {
    glm::vec3 ambient{glm::vec3(1.f)};
    glm::vec3 diffuse{glm::vec3(1.f)};
    glm::vec3 specular{glm::vec3(1.f)};
    float shininess{32.f};

    Material() = default;
    explicit Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh)
        : ambient(std::move(a)), diffuse(std::move(d)), specular(std::move(s)), shininess(sh) {}
};

struct Skybox {
    bool useFog{};
    glm::vec3 fogColor{};
    float fogDensity{};
    float fogGradient{};
    glm::vec3 skyColor{};
    float rotationSpeed{};

    Skybox() = default;
    explicit Skybox(bool uf, glm::vec3 fc, float fd, float fg, glm::vec3 sc, float rs)
        : useFog(uf), fogColor(std::move(fc)), fogDensity(fd), fogGradient(fg), skyColor(std::move(sc)), rotationSpeed(rs) {}
};

struct TimeComponent {
    float seconds{};
    int currentHour{};
    int currentMinute{};
    int currentSecond{};
    float startingTime{};
    float dayLength{};
    float nightStart{};
    float dayTransitionStart{};
    float dayStart{};
    float nightTransitionStart{};
    float acceleration{};
    int fps{60};

    TimeComponent() = default;
    explicit TimeComponent(float s, int ch, int cm, int cs, float st, float dl, float ns, float dts, float ds, float nts, float a, int f)
        : seconds(s), currentHour(ch), currentMinute(cm), currentSecond(cs), startingTime(st), dayLength(dl), nightStart(ns), dayTransitionStart(dts), dayStart(ds), nightTransitionStart(nts), acceleration(a), fps(f) {}
};

struct TextureAtlas {
    int rows{};
    int index{};

    TextureAtlas() = default;
    explicit TextureAtlas(int r, int i) : rows(r), index(i) {}
};

struct TextureComponent {
    std::vector<std::shared_ptr<Texture>> textures;
    bool hasTransparency{};
    bool useFakeLighting{};
    bool useBlending{};
    float blendFactor{};
    bool useColor{};
    glm::vec4 color{};
    bool useReflection{};
    float reflectivity{};
    bool useRefraction{};
    float refractiveIndex{};

    TextureComponent() = default;
    explicit TextureComponent(std::vector<std::shared_ptr<Texture>> t, bool ht, bool ufl, bool ub, float bf, bool uc, glm::vec4 c, bool ur, float r, bool urf, float ri)
        : textures(std::move(t)), hasTransparency(ht), useFakeLighting(ufl), useBlending(ub), blendFactor(bf), useColor(uc), color(c), useReflection(ur), reflectivity(r), useRefraction(urf), refractiveIndex(ri) {}
};

struct ShaderProgramComponent {
    std::string shaderProgram{};

    ShaderProgramComponent() = default;
    explicit ShaderProgramComponent(std::string sp) : shaderProgram(std::move(sp)) {}
};

struct Mesh {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<VAO> vao;

    Mesh() = default;
    explicit Mesh(std::vector<Vertex> v, std::vector<uint32_t> i, std::vector<std::shared_ptr<engine::Texture>> t)
        : vertices(std::move(v)), indices(std::move(i)), textures(t) {}

    void setupMesh() {
        vao = VAO::Create();
        vao->attachVertex(VBO::Create(vertices));
        vao->setIndex(IBO::Create(indices));
    }

    const std::shared_ptr<VAO>& getVAO() {
        if (not vao) {
            setupMesh();
        }
        return vao;
    }

    void bindTextures(std::unique_ptr<ShaderProgram>& sp, TextureComponent* cTexture, TextureAtlas* cTextureAtlas, Skybox* cSkybox, Skybox* _cSkybox, TextureComponent* _cSkyboxTexture, const glm::vec3& lightPosition, const glm::vec4& lightColor, const glm::vec3& cameraPosition, Material* cMaterial) {
        sp->resetActiveUniforms();
        sp->use();
        uint32_t i = 1;
        if (sp->getName() == "basic") {
            if (_cSkybox and _cSkybox->useFog) {
                sp->setFloat("useFog", 1.f);
                sp->setFloat("fogDensity", _cSkybox->fogDensity);
                sp->setFloat("fogGradient", _cSkybox->fogGradient);
                sp->setVec3("skyColor", _cSkybox->skyColor);
            }
        }
        if (cTexture) {
            for (auto& texture : cTexture->textures) {
                if (cTexture->useFakeLighting) {
                    sp->setFloat("useFakeLighting", 1.f);
                    sp->setVec3("lightPosition", lightPosition);
                    sp->setVec4("lightColor", lightColor);
                }
                if (cTextureAtlas) {
                    sp->setFloat("useAtlas", 1.f);
                    int index = cTextureAtlas->index;
                    int rows = cTextureAtlas->rows;
                    sp->setFloat("numRows", rows);
                    int col = index % rows;
                    float coll = static_cast<float>(col) / static_cast<float>(rows);
                    int row = index / rows;
                    float roww = static_cast<float>(row) / static_cast<float>(rows);
                    sp->setVec2("offset", glm::vec2(coll, roww));
                }
                if (cTexture->useBlending) {
                    sp->setFloat("useBlending", 1.f);
                    sp->setFloat("blendFactor", cTexture->blendFactor);
                }
                if (cTexture->useColor) {
                    sp->setFloat("useColor", 1.f);
                    sp->setVec4("color", cTexture->color);
                }
                if (cSkybox) {
                    sp->setFloat("useFog", cSkybox->useFog);
                    sp->setVec3("fogColor", cSkybox->fogColor);
                }
                if (_cSkybox and _cSkyboxTexture) {
                    if (_cSkyboxTexture->useBlending) {
                        sp->setFloat("useSkyBlending", 1.f);
                        sp->setFloat("skyBlendFactor", _cSkyboxTexture->blendFactor);
                    }
                    int ti = 10;
                    for (auto& t : _cSkyboxTexture->textures) {
                        sp->setInt(t->getType().data() + std::string("Sky") + std::to_string(ti), ti);
                        t->bindSlot(ti);
                        ti++;
                    }
                    if (cTexture->useReflection) {
                        sp->setFloat("useReflection", 1.f);
                        sp->setFloat("reflectivity", cTexture->reflectivity);
                        sp->setVec3("cameraPosition", cameraPosition);
                    }
                    if (cTexture->useRefraction) {
                        sp->setFloat("useRefraction", 1.f);
                        sp->setFloat("refractiveIndex", cTexture->refractiveIndex);
                    }
                }
                if (cMaterial) {
                    sp->setVec3("ambient", cMaterial->ambient);
                    sp->setVec3("diffuse", cMaterial->diffuse);
                    sp->setVec3("specular", cMaterial->specular);
                    sp->setFloat("shininess", cMaterial->shininess);
                }

                sp->setInt(texture->getType().data() + std::to_string(i), i);
                texture->bindSlot(i);
                ++i;
            }
        } else {
            uint32_t diffuseN = 1;
            uint32_t specularN = 1;
            uint32_t normalN = 1;
            uint32_t heightN = 1;
            for (auto& texture : textures) {
                std::string number;
                std::string_view type = texture->getType();
                if (type == "textureDiffuse") {
                    number = std::to_string(diffuseN++);
                } else if (type == "textureSpecular") {
                    number = std::to_string(specularN++);
                } else if (type == "textureNormal") {
                    number = std::to_string(normalN++);
                } else if (type == "textureHeight") {
                    number = std::to_string(heightN++);
                } else {
                    throw std::runtime_error("Unknown texture type " + std::string(type));
                }
                sp->setInt(type.data() + number, i);
                texture->bindSlot(i);
                ++i;
            }
            if (diffuseN == 1) {
                sp->setFloat("useNormal", 1.f);
            }
        }
        sp->unuse();
    }

    void unbindTextures(TextureComponent* cTexture) {
        auto& texturesToUnbind = (cTexture) ? cTexture->textures : textures;
        for (auto& texture : texturesToUnbind) {
            texture->unbindSlot();
        }
    }
};

struct Body {
    std::string filepath{};
    std::vector<Mesh> meshes{};
    std::vector<Material> materials{};

    Body() = default;
    explicit Body(std::string fp, std::vector<Mesh> m, std::vector<Material> ma)
        : filepath(std::move(fp)), meshes(std::move(m)), materials(std::move(ma)) {}
};

struct RigidBody {
    float mass{};
    float friction{};
    float bounciness{};
    bool isKinematic{};

    RigidBody() = default;
    explicit RigidBody(float m, float f, float b, bool k) : mass(m), friction(f), bounciness(b), isKinematic(k) {}
};

struct Collider {
    enum class Type {
        Box,
        Capsule,
        Mesh,
        Sphere,
    };

    Type type;
    glm::vec3 size = glm::vec3(1.f);

    Collider() = default;
    explicit Collider(Type t, glm::vec3 s) : type(t), size(s) {}
};

struct CameraComponent {
    Camera camera;  // TODO maybe remove default constructor

    CameraComponent() = default;
    explicit CameraComponent(Camera c) : camera(std::move(c)) {}
};

struct Light {
    enum class Type {  // https://docs.unity3d.com/Manual/Lighting.html
        Point,
        Spot,
        Directional,
        Area,
    };

    Type type;
    glm::vec4 color = glm::vec4(1.f);
    float intensity{};
    float range{};
    float innerConeAngle{};
    float outerConeAngle{};

    Light() = default;
    explicit Light(Type t, glm::vec4 c, float i, float r, float ica, float oca)
        : type(t), color(c), intensity(i), range(r), innerConeAngle(ica), outerConeAngle(oca) {}

    std::string to_string(Type t) noexcept {
        using enum Type;
        switch (t) {
            case Point:
                return "Point";
            case Spot:
                return "Spot";
            case Directional:
                return "Directional";
            case Area:
                return "Area";
        }
    }
};

struct Audio {
    // TODO Audio class
    std::string filepath{};
    float volume{};
    float pitch{};
    bool loop{};

    Audio() = default;
    explicit Audio(std::string fp, float v, float p, bool l) : filepath(std::move(fp)), volume(v), pitch(p), loop(l) {}
};

struct ParticleComponent {
    // TODO define
    std::string emitter{};

    ParticleComponent() = default;
    explicit ParticleComponent(std::string e) : emitter(std::move(e)) {}
};

struct Animation {
    // TODO define
    std::string filepath{};

    Animation() = default;
    explicit Animation(std::string fp) : filepath(std::move(fp)) {}
};

struct Text {
    // font class or freetype?
    std::string text{};
    glm::vec4 color = glm::vec4(1.f);  // white

    Text() = default;
    explicit Text(std::string t, glm::vec4 c) : text(std::move(t)), color(c) {}
};

struct AI {
    // TODO define
    std::string filepath{};

    AI() = default;
    explicit AI(std::string fp) : filepath(std::move(fp)) {}
};

struct Item {
    // TODO define
    std::string name{};
    std::string description{};
    std::string icon{};
    std::string model{};
    int value{};

    Item() = default;
    explicit Item(std::string n, std::string d, std::string i, std::string m, int v)
        : name(std::move(n)), description(std::move(d)), icon(std::move(i)), model(std::move(m)), value(v) {}
};

struct Inventory {
    // TODO define
    std::vector<Item> items{};

    Inventory() = default;
    explicit Inventory(std::vector<Item> i) : items(std::move(i)) {}
};

struct Relationship {
    entt::entity parent{entt::null};

    Relationship() = default;
    explicit Relationship(entt::entity p) : parent(p) {}
};

struct Health {
    int base{};
    int current{};

    Health() = default;
    explicit Health(int b) : base(b), current(b) {}
};

struct Mana {
    int base{};
    int current{};

    Mana() = default;
    explicit Mana(int b) : base(b), current(b) {}
};

struct Stamina {
    int base{};
    int current{};

    Stamina() = default;
    explicit Stamina(int b) : base(b), current(b) {}
};

struct Experience {
    int current{};

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

    Equipment() = default;
    explicit Equipment(std::string h, std::string n, std::string s, std::string c, std::string b, std::string ha, std::string lf, std::string rf, std::string be, std::string l, std::string f)
        : head(std::move(h)), neck(std::move(n)), shoulders(std::move(s)), chest(std::move(c)), back(std::move(b)), hands(std::move(ha)), lfinger(std::move(lf)), rfinger(std::move(rf)), belt(std::move(be)), legs(std::move(l)), feet(std::move(f)) {}
};

struct Stats {
    int strength{};
    int dexterity{};
    int constitution{};
    int intelligence{};
    int wisdom{};
    int charisma{};

    Stats() = default;
    explicit Stats(int s, int d, int c, int i, int w, int ch)
        : strength(s), dexterity(d), constitution(c), intelligence(i), wisdom(w), charisma(ch) {}
};

struct Talents {
    int acrobatics{};
    int arcana{};
    int athletics{};
    int perception{};
    int persuasion{};
    int stealth{};
    int survival{};
    int luck{};

    Talents() = default;
    explicit Talents(int a, int ar, int at, int p, int pe, int s, int su, int l)
        : acrobatics(a), arcana(ar), athletics(at), perception(p), persuasion(pe), stealth(s), survival(su), luck(l) {}
};

struct Skills {
    int mining{};
    int jewelcrafting{};
    int blacksmithing{};
    int hunting{};
    int fishing{};
    int skinning{};
    int leatherworking{};
    int herbalism{};
    int cooking{};
    int alchemy{};
    int enchanting{};
    int harvesting{};
    int tailoring{};
    int woodworking{};
    int woodcutting{};
    int farming{};
    int quarrying{};
    int masonry{};

    Skills() = default;
    explicit Skills(int m, int j, int b, int h, int f, int sk, int l, int he, int c, int a, int e, int ha, int t, int w, int wc, int fa, int q, int ma)
        : mining(m), jewelcrafting(j), blacksmithing(b), hunting(h), fishing(f), skinning(sk), leatherworking(l), herbalism(he), cooking(c), alchemy(a), enchanting(e), harvesting(ha), tailoring(t), woodworking(w), woodcutting(wc), farming(fa), quarrying(q), masonry(ma) {}
};

}