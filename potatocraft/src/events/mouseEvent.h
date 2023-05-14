#pragma once

#include "src/events/event.h"
#include "src/core/mouseCodes.h"

namespace potatoengine
{

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const double x, const double y)
			: m_mouseX(x), m_mouseY(y) {}

		double getX() const { return m_mouseX; }
		double getY() const { return m_mouseY; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		double m_mouseX, m_mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const double x, const double y)
			: m_mouseX(x), m_mouseY(y) {}

		double getX() const { return m_mouseX; }
		double getY() const { return m_mouseY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		double m_mouseX, m_mouseY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(const MouseCode button)
			: m_button(button) {}

		MouseCode m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}