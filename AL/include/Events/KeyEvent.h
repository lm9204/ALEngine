#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Core/KeyCodes.h"
#include "Events/Event.h"

namespace ale
{
class KeyEvent : public Event
{
  public:
	KeyCode getKeyCode()
	{
		return m_KeyCode;
	}

	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)
  protected:
	KeyEvent(const KeyCode keycode) : m_KeyCode(keycode)
	{
	}

	KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
  public:
	KeyPressedEvent(const KeyCode keycode, bool isRepeat = false) : KeyEvent(keycode), m_IsRepeat(isRepeat)
	{
	}

	bool isRepeat()
	{
		return m_IsRepeat;
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_PRESSED)

  protected:
	bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
  public:
	KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode)
	{
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_RELEASED)
};

class KeyTypedEvent : public KeyEvent
{
  public:
	KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode)
	{
	}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KEY_TYPED)
};

} // namespace ale

#endif