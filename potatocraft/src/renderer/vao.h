#pragma once

#include "src/renderer/buffer.h"

namespace potatoengine {

	class VAO
	{
	public:
		VAO();
		~VAO();

		void bind() const;

		void attachVertex(const Ref<VBO>& vbo);
		void setIndex(const Ref<IBO>& ibo);

		const std::vector<Ref<VBO>>& getVBOs() const { return m_vbos; }
		const Ref<IBO>& getEBO() const { return m_ibo; }

		static Ref<VAO> Create();
	private:
		uint32_t m_id;
		std::vector<Ref<VBO>> m_vbos;
		Ref<IBO> m_ibo;
		uint32_t m_vboIndex = 0;
	};

}