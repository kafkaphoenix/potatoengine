#pragma once

#include "src/core/keyCodes.hpp"
#include "src/core/mouseCodes.hpp"

#include <glm/glm.hpp>

namespace potatocraft {

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}