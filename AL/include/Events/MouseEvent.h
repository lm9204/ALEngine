#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Core/MouseCodes.h"
#include "Events/Event.h"

namespace ale
{
class AL_API MouseMovedEvent : public Event
{
  public:
	MouseMovedEvent(const float x, const float y) : m_MouseX(x), m_MouseY(y)
	{
	}

	float getX() const
	{
		return m_MouseX;
	}

	float getY() const
	{
		return m_MouseY;
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_MOVE);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);

  private:
	float m_MouseX, m_MouseY;
};

class AL_API MouseScrolledEvent : public Event
{
  public:
	MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset)
	{
	}

	float getXOffset() const
	{
		return m_XOffset;
	}

	float getYOffset() const
	{
		return m_YOffset;
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_SCROLLED);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);

  private:
	float m_XOffset, m_YOffset;
};

class AL_API MouseButtonEvent : public Event
{
  public:
	MouseCode getMouseButton() const
	{
		return m_Button;
	}

	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE_BUTTON);

  protected:
	MouseButtonEvent(const MouseCode button) : m_Button(button)
	{
	}

	MouseCode m_Button;
};

class AL_API MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
	MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button)
	{
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED);
};

class AL_API MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
	MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button)
	{
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED);
};

} // namespace ale

#endif