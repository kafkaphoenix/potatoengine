#pragma once

#include "src/core/keyCodes.h"
#include "src/core/mouseCodes.h"

#include <glm/glm.hpp>

namespace potatoengine {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}