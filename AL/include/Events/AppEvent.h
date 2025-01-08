#ifndef APPEVENT_H
#define APPEVENT_H

#include "Events/Event.h"

namespace ale
{
class WindowResizeEvent : public Event
{
  public:
	WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
	{
	}

	unsigned int getWidth() const
	{
		return m_Width;
	}

	unsigned int getHeight() const
	{
		return m_Height;
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WINDOW_RESIZE);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);

  private:
	unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
  public:
	WindowCloseEvent() = default;

	EVENT_CLASS_TYPE(WINDOW_CLOSE);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
};

class AppTickEvent : public Event
{
  public:
	AppTickEvent() = default;

	EVENT_CLASS_TYPE(APP_TICK);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
};

class AppUpdateEvent : public Event
{
  public:
	AppUpdateEvent() = default;

	EVENT_CLASS_TYPE(APP_UPDATE);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
};

class AppRenderEvent : public Event
{
  public:
	AppRenderEvent() = default;

	EVENT_CLASS_TYPE(APP_RENDER);
	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP);
};

} // namespace ale

#endif