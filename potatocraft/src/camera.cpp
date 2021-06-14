#include <algorithm>
#include <src/camera.hpp>

void Camera::rotate(glm::vec3 axis, float angle) {
    glm::quat rot = glm::angleAxis(angle, axis);
    glm::quat orientation = rot * m_orientation;
    m_orientation = glm::normalize(orientation);
}

void Camera::strafe(float amount) {
    move({0.f, amount, 0.f});
}

void Camera::climb(float amount) {
    move({0.f, 0.f, amount});
}

void Camera::advance(float amount) {
    move({amount, 0.f, 0.f});
}

void Camera::move(glm::vec3 delta) {
    // conjugation to get inverted orientation
    auto tmp = glm::mat4_cast(m_orientation);
    auto x = glm::vec3(tmp[0][2], tmp[1][2], tmp[2][2]);
    auto y = -glm::vec3(tmp[0][0], tmp[1][0], tmp[2][0]);
    auto z = -glm::vec3(tmp[0][1], tmp[1][1], tmp[2][1]);
    // rot * translation
    m_position += x*delta.x + y*delta.y + z*delta.z;
    // std::cout << glm::to_string(m_position) << std::endl;
}

glm::mat4 Camera::get_view() {
    auto tmp = glm::mat4_cast(m_orientation);
    tmp = glm::translate(tmp, m_position);
    return tmp;
}

void Camera::processKeyboard(CameraMovement direction, float dt) {
    float velocity = dt * m_movement_speed;
    if (direction == CameraMovement::FORWARD)
        rotate({-1.f, 0.f, 0.f}, velocity);

    if (direction == CameraMovement::BACKWARD)
        rotate({1.f, 0.f, 0.f}, velocity);

    if (direction == CameraMovement::LEFT)
        rotate({0.f, -1.f, 0.f}, velocity);

    if (direction == CameraMovement::RIGHT)
        rotate({0.f, 1.f, 0.f}, velocity);

    if (direction == CameraMovement::LEAN_LEFT)
        rotate({0.f, 0.f, -1.f}, velocity);

    if (direction == CameraMovement::LEAN_RIGHT)
        rotate({0.f, 0.f, 1.f}, velocity);

    if (direction == CameraMovement::ZOOM_IN)
        advance(dt * m_fov);

    if (direction == CameraMovement::ZOOM_OUT)
        advance(dt * -m_fov);

    if (direction == CameraMovement::ROLL_LEFT)
        strafe(dt * -m_roll_power);

    if (direction == CameraMovement::ROLL_RIGHT)
        strafe(dt * m_roll_power);

    if (direction == CameraMovement::JUMP)
        climb(dt * m_jump_power);

    if (direction == CameraMovement::CROUCH)
        climb(dt * -m_crouch_power);
}

void Camera::processMouseMovement(double xoffset, double yoffset, bool constrainPitch) {
    xoffset *= m_mouse_sensivity;
    yoffset *= m_mouse_sensivity;
    m_right_angle += xoffset;
    m_up_angle += yoffset;

    m_right_angle = std::fmod(m_right_angle, 360.f);
    m_up_angle = std::clamp<float>(m_up_angle, -89.f, 89.f);
    // std::cout << m_right_angle << std::endl;
    // std::cout << m_up_angle << std::endl;

    // yaw mouse movement in x-direction
    glm::quat rotY = glm::angleAxis(glm::radians(m_right_angle), glm::vec3(0, 1, 0));
    // pitch mouse movement in y-direction
    glm::quat rotX = glm::angleAxis(glm::radians(-m_up_angle), glm::vec3(1, 0, 0));

    m_orientation = rotY * rotX;
    // std::cout << glm::to_string(m_orientation) << std::endl;
}

void Camera::processMouseScroll(double yoffset) {
    if (m_fov >= MIN_FOV && m_fov <= MAX_FOV)
        m_fov -= yoffset;

    if (m_fov <= MIN_FOV)
        m_fov = MIN_FOV;

    if (m_fov >= MAX_FOV)
        m_fov = MAX_FOV;
}

const float& Camera::getFov() const noexcept {
    return m_fov;
}

Camera::Camera(const glm::vec3& position, const glm::vec3& orientation):
    m_position(position), m_orientation(orientation), m_movement_speed(SPEED),
    m_mouse_sensivity(SENSITIVITY), m_roll_power(ROLL_POWER), m_fov(MAX_FOV),
    m_jump_power(JUMP_POWER), m_crouch_power(CROUCH_POWER), m_right_angle(0.f),
    m_up_angle(0.f) {
}