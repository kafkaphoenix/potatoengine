#pragma once

#include "src/core/base.hpp"
#include "src/core/timestep.hpp"
#include "src/event/event.hpp"

namespace potatocraft {

	class State
	{
	public:
		State(const std::string& name = "State");
		virtual ~State() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		const std::string& getName() const { return m_debugName; }
	protected:
		std::string m_debugName;
	};

}