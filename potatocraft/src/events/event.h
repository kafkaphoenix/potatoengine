#pragma once

#include "src/pch.h"

namespace potatoengine
{

	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum EventCategory
	{
		// Each category is a bit flag
		None = 0,
		EventCategoryApplication = 1,
		EventCategoryInput = 2,
		EventCategoryKeyboard = 4,
		EventCategoryMouse = 8,
		EventCategoryMouseButton = 16,
	};

#define EVENT_CLASS_TYPE(type)                                                  \
	static EventType GetStaticType() { return EventType::type; }                \
	virtual EventType getEventType() const override { return GetStaticType(); } \
	virtual const char *getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool m_handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char *getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event &event)
			: m_event(event)
		{
		}

		// F will be deduced by the compiler
		template <typename T, typename F>
		bool dispatch(const F &func)
		{
			if (m_event.getEventType() == T::GetStaticType())
			{
				m_event.m_handled |= func(static_cast<T &>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event &m_event;
	};

}