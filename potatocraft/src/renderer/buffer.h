#pragma once

#include <glm/glm.hpp>

#include "src/pch.h"

namespace potatoengine {

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec4 col;
		glm::vec2 tex;
	};

	class VBO
	{
	public:

		VBO(const std::vector<Vertex>& vertices);
		~VBO();

		uint32_t getId() const { return m_id; }
		const std::vector<Vertex>& getVertices() const { return m_vertices; }

		static std::shared_ptr<VBO> Create(const std::vector<Vertex>& vertices);

	private:
		std::vector<Vertex> m_vertices;
		uint32_t m_id;
	};

	class IBO
	{
	public:
		IBO(const std::vector<uint32_t>& indices);
		~IBO();

		uint32_t getCount() const { return m_count; }
		uint32_t getId() const { return m_id; }

		static std::shared_ptr<IBO> Create(const std::vector<uint32_t>& indices);
	private:
		uint32_t m_id;
		uint32_t m_count;
	};
}