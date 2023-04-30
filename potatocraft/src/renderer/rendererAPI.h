#pragma once

#include "src/renderer/vao.h"

#include <glm/glm.hpp>

namespace potatoengine {
	class RendererAPI
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void SetWireframe(bool enabled);
		static void Clear();
		static void DrawIndexed(const Ref<VAO>& vao);
	};
}