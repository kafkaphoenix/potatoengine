#pragma once

#include "src/core/base.hpp"
#include "src/core/state.hpp"

#include <vector>

namespace potatocraft {

	class StateStack
	{
	public:
		StateStack() = default;
		~StateStack();

		void pushState(State* state);
		void pushOverlay(State* overlay);
		void popState(State* state);
		void popOverlay(State* overlay);

		std::vector<State*>::iterator begin() { return m_states.begin(); }
		std::vector<State*>::iterator end() { return m_states.end(); }
		std::vector<State*>::reverse_iterator rbegin() { return m_states.rbegin(); }
		std::vector<State*>::reverse_iterator rend() { return m_states.rend(); }

		std::vector<State*>::const_iterator begin() const { return m_states.begin(); }
		std::vector<State*>::const_iterator end()	const { return m_states.end(); }
		std::vector<State*>::const_reverse_iterator rbegin() const { return m_states.rbegin(); }
		std::vector<State*>::const_reverse_iterator rend() const { return m_states.rend(); }
	private:
		std::vector<State*> m_states;
		unsigned int m_stateInsertIndex = 0;
	};

}