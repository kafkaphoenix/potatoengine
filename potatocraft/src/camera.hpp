#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera
{
public:

	void rotate(glm::vec3 axis, float angle) {
		glm::quat rot = glm::angleAxis(angle, axis);
		glm::quat orientation = rot * m_orientation;
		m_orientation = normalize(orientation);
	}

	void strafe(float amount) {
		move({0.f, amount, 0.f});
	}

	void climb(float amount) {
		move({0.f, 0.f, amount});
	}

	void advance(float amount) {
		move({amount, 0.f, 0.f});
	}

	void move(glm::vec3 delta) {
		auto tmp = glm::mat4_cast(m_orientation);
		auto x = glm::vec3(tmp[0][2], tmp[1][2], tmp[2][2]);
		auto y = -glm::vec3(tmp[0][0], tmp[1][0], tmp[2][0]);
		auto z = -glm::vec3(tmp[0][1], tmp[1][1], tmp[2][1]);
		m_position += x*delta.x + y*delta.y + z*delta.z;
		//std::cout << glm::to_string(m_position) << std::endl;
	}

	glm::mat4 get_view() {
		auto tmp = glm::mat4_cast(m_orientation);
		tmp = glm::translate(tmp, m_position);
		return tmp;
	}

	Camera(const glm::vec3 &position, const glm::vec3 &orientation);

private:
    glm::quat m_orientation;
	glm::vec3 m_position;
};

#endif // CAMERA_H_INCLUDED