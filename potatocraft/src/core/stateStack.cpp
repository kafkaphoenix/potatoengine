#include "src/pcpch.hpp"
#include "src/core/stateStack.hpp"

namespace potatocraft {

	StateStack::~StateStack()
	{
		for (auto state : m_states)
		{
			state->onDetach();
			delete state;
		}
	}

	void StateStack::pushState(State* state)
	{
		m_states.emplace(m_states.begin() + m_stateInsertIndex, state);
		m_stateInsertIndex++;
	}

	void StateStack::pushOverlay(State* overlay)
	{
		m_states.emplace_back(overlay);
	}

	void StateStack::popState(State* state)
	{
		auto it = std::find(m_states.begin(), m_states.begin() + m_stateInsertIndex, state);
		if (it != m_states.begin() + m_stateInsertIndex)
		{
			state->onDetach();
			m_states.erase(it);
			m_stateInsertIndex--;
		}
	}

	void StateStack::popOverlay(State* overlay)
	{
		auto it = std::find(m_states.begin() + m_stateInsertIndex, m_states.end(), overlay);
		if (it != m_states.end())
		{
			overlay->onDetach();
			m_states.erase(it);
		}
	}

}
