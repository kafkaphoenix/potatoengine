#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	LEAN_LEFT,
	LEAN_RIGHT,
	ZOOM_IN,
	ZOOM_OUT,
	ROLL_LEFT,
	ROLL_RIGHT,
	JUMP,
	CROUCH
};

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ROLL_POWER = 45.0f;
const float ZOOM = 45.0f;
const float JUMP_POWER = 45.0f;
const float CROUCH_POWER = 45.0f;
const bool ZOOM_ENABLED = false;

class Camera
{
private:
	glm::vec3 m_position;
    glm::quat m_orientation;
	float m_movement_speed;
	float m_mouse_sensivity;
	float m_roll_power;
	float m_zoom;
	float m_jump_power;
	float m_crouch_power;
	bool m_zoom_enabled;

public:
	void rotate(glm::vec3 axis, float angle);
	void strafe(float amount);
	void climb(float amount);
	void advance(float amount);
	void move(glm::vec3 delta);
	glm::mat4 get_view();
	void processKeyboard(CameraMovement direction, float dt);
	void processMouseMovement(double xoffset, double yoffset, bool constrainPitch=true);
	void processMouseScroll(double yoffset);

	Camera(const glm::vec3 &position, const glm::vec3 &orientation);
};

#endif // CAMERA_H_INCLUDED