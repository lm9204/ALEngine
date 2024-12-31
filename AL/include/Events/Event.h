#ifndef EVENT_H
#define EVENT_H

#include "Core/Base.h"
#include <sstream>

namespace ale
{
enum class EEventType
{
	NONE = 0,
	WINDOW_CLOSE,
	WINDOW_RESIZE,
	WINDOW_FOCUS,
	WINDOW_LOST_FOCUS,
	WINDOW_MOVED, // window
	APP_TICK,
	APP_UPDATE,
	APP_RENDER, // app
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_TYPED, // key
	MOUSE_BUTTON_PRESSED,
	MOUSE_BUTTON_RELEASED,
	MOUSE_MOVE,
	MOUSE_SCROLLED // mouse
};

enum EEventCategory
{
	NONE = 0,
	EVENT_CATEGORY_APP = BIT(0),
	EVENT_CATEGORY_INPUT = BIT(1),
	EVENT_CATEGORY_KEYBOARD = BIT(2),
	EVENT_CATEGORY_MOUSE = BIT(3),
	EVENT_CATEGORY_MOUSE_BUTTON = BIT(4)
};

// instance 없이 type get
// #은 매크로에서 토큰을 문자열로 변환하는 데 사용됨
#define EVENT_CLASS_TYPE(type)                                                                                         \
	static EEventType getStaticType()                                                                                  \
	{                                                                                                                  \
		return EEventType::type;                                                                                       \
	}                                                                                                                  \
	virtual EEventType getEventType() const override                                                                   \
	{                                                                                                                  \
		return getStaticType();                                                                                        \
	}                                                                                                                  \
	virtual const char *getName() const override                                                                       \
	{                                                                                                                  \
		return #type;                                                                                                  \
	}

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
	virtual int getCategoryFlags() const override                                                                      \
	{                                                                                                                  \
		return category;                                                                                               \
	}

// Event base class
class AL_API Event
{
	friend class EventDispatcher;

  public:
	virtual ~Event() = default;

	virtual EEventType getEventType() const = 0;
	virtual const char *getName() const = 0;
	virtual int getCategoryFlags() const = 0;
	virtual std::string toString() const
	{
		return getName();
	}

	inline bool isInCategory(EEventCategory category)
	{
		return getCategoryFlags() & category;
	}

	bool m_Handled = false;
};

// 발생한 이벤트를 핸들하기 위한 클래스
class EventDispatcher
{
  public:
	EventDispatcher(Event &event) : m_Event(event)
	{
	}

	// 콜백 함수
	template <typename T, typename F> bool dispatch(const F &func)
	{
		// F는 컴파일러에 의해 추론
		if (m_Event.getEventType() == T::getStaticType())
		{
			m_Event.m_Handled |= func(static_cast<T &>(m_Event));
			return true;
		}
		return false;
	}

  private:
	Event &m_Event;
};

inline std::ostream &operator<<(std::ostream &os, const Event &e)
{
	return os << e.toString();
}

} // namespace ale

#endif
