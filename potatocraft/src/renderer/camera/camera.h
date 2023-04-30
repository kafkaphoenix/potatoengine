#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// pitch (rot around x in radians)
// yaw (rot around y in radians)
// roll (rot around z in radians)

const float SPEED = 10.f;
const float SENSITIVITY = 0.1f;
const float MAX_FOV = 90.f;
const float MIN_FOV = 1.f;
const float JUMP_POWER = 45.f;

namespace potatoengine
{

	class Camera
	{
	public:
		Camera(const glm::vec3 &position, const glm::vec3 &orientation);

		enum class CameraMovement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			JUMP,
			CROUCH
		};

		const glm::mat4 getView() const noexcept;
		void processKeyboard(CameraMovement direction, float dt);
		void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void processMouseScroll(float yoffset);
		const float &getFov() const noexcept;

	private:
		glm::vec3 m_position;
		glm::quat m_orientation;
		float m_right_angle{};
		float m_up_angle{};
		float m_movement_speed;
		float m_mouse_sensivity;
		float m_fov;
		float m_jump_power;
	};

}