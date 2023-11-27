#pragma once

namespace potatoengine {

struct CRigidBody {
    float mass{};
    float friction{};
    float bounciness{};
    bool isKinematic{};

    CRigidBody() = default;
    explicit CRigidBody(float m, float f, float b, bool k) : mass(m), friction(f), bounciness(b), isKinematic(k) {}

    void print() const {
        CORE_TRACE("\t\tmass: {0}\n\t\t\t\tfriction: {1}\n\t\t\t\tbounciness: {2}\n\t\t\t\tisKinematic: {3}", mass, friction, bounciness, isKinematic);
    }
};
}