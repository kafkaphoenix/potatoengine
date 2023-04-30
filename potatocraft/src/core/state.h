#pragma once

#include "src/core/base.h"
#include "src/core/timestep.h"
#include "src/event/event.h"

namespace potatoengine {

	class State
	{
	public:
		State(const std::string& name="TemplateState");
		virtual ~State() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		const std::string& getName() const { return m_name; }
	protected:
		std::string m_name;
	};

}