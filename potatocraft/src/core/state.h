#pragma once

#include "src/core/time.h"
#include "src/events/event.h"

namespace potatoengine {

	class State
	{
	public:
		State(const std::string& name="TemplateState");
		virtual ~State() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Time dt) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		const std::string& getName() const { return m_name; }
	protected:
		std::string m_name;
	};

}