#include <algorithm>

#include <src/renderer/camera/camera.hpp>

glm::mat4 Camera::get_view() {
    glm::quat reverseOrient = glm::conjugate(m_orientation);
    glm::mat4 rot = glm::mat4_cast(reverseOrient);
    rot[3][0] = -(rot[0][0] * m_position.x + rot[1][0] * m_position.y + rot[2][0] * m_position.z);
    rot[3][1] = -(rot[0][1] * m_position.x + rot[1][1] * m_position.y + rot[2][1] * m_position.z);
    rot[3][2] = -(rot[0][2] * m_position.x + rot[1][2] * m_position.y + rot[2][2] * m_position.z);
    rot[3][3] = 1;

    return rot;
}

void Camera::processKeyboard(CameraMovement direction, float dt) {
    float velocity = dt * m_movement_speed;

    glm::quat qF = m_orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(m_orientation);
    glm::vec3 front = { qF.x, qF.y, qF.z };
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    glm::vec3 up = {0, dt * m_jump_power, 0};

    if (direction == CameraMovement::FORWARD)
        m_position += front * velocity;

    if (direction == CameraMovement::BACKWARD)
        m_position -= front * velocity;

    if (direction == CameraMovement::LEFT)
        m_position -= right * velocity;

    if (direction == CameraMovement::RIGHT)
        m_position += right * velocity;

    if (direction == CameraMovement::JUMP)
        m_position += up;

    if (direction == CameraMovement::CROUCH)
        m_position -= up;
}

void Camera::processMouseMovement(double xoffset, double yoffset, bool constrainPitch) {
    xoffset *= m_mouse_sensivity;
    yoffset *= m_mouse_sensivity;
    m_right_angle += xoffset;
    m_up_angle += yoffset;

    m_right_angle = std::fmod(m_right_angle, 360.f);
    m_up_angle = std::clamp<float>(m_up_angle, -89.f, 89.f);

    // yaw mouse movement in x-direction
    glm::quat rotY = glm::angleAxis(glm::radians(-m_right_angle), glm::vec3(0, 1, 0));
    // pitch mouse movement in y-direction
    glm::quat rotX = glm::angleAxis(glm::radians(m_up_angle), glm::vec3(1, 0, 0));

    m_orientation = rotY * rotX;
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
    m_mouse_sensivity(SENSITIVITY), m_fov(MAX_FOV), m_jump_power(JUMP_POWER),
    m_right_angle(0.f), m_up_angle(0.f) {
}