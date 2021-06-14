#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

// references
// http://chanhaeng.blogspot.com/2018/09/quaternion-camera-implementation.html
// https://stackoverflow.com/questions/49609654/quaternion-based-first-person-view-camera


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

// pitch (rot around x in radians), 
// yaw (rot around y in radians), 
// roll (rot around z in radians)

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ROLL_POWER = 45.f;
const float MAX_FOV = 90.f;
const float MIN_FOV = 1.f;
const float JUMP_POWER = 45.f;
const float CROUCH_POWER = 45.f;

class Camera
{
private:
	glm::vec3 m_position;
    glm::quat m_orientation;
	float m_right_angle;
	float m_up_angle;
	float m_movement_speed;
	float m_mouse_sensivity;
	float m_roll_power;
	float m_fov;
	float m_jump_power;
	float m_crouch_power;

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
	const float &getFov() const noexcept;

	Camera(const glm::vec3 &position, const glm::vec3 &orientation);
};

#endif // CAMERA_H_INCLUDED