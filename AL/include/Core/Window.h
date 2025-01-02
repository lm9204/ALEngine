#ifndef WINDOW_H
#define WINDOW_H

#include "ALpch.h"
#include "Core/Base.h"
#include "Events/Event.h"
#include <GLFW/glfw3.h>
#include "Renderer/Scene.h"

namespace ale
{
struct WindowProps
{
	std::string title;
	uint32_t width;
	uint32_t height;

	WindowProps(const std::string &title = "ALEngine", unsigned int width = WINDOW_WIDTH,
				unsigned int height = WINDOW_HEIGHT)
		: title(title), width(width), height(height)
	{
	}
};

// window base class
// can be implemented different by platforms
class AL_API Window
{
  public:
	using EventCallbackFn = std::function<void(Event &)>;

	virtual ~Window() = default;

	virtual void onUpdate() = 0;
	virtual uint32_t getWidth() const = 0;
	virtual uint32_t getHeight() const = 0;
	virtual GLFWwindow *getNativeWindow() const = 0;

	virtual void setEventCallback(const EventCallbackFn &callback) = 0;
	virtual void setVSync(bool enabled) = 0;
	virtual bool isVSync() const = 0;
	virtual void bindScene(Scene *scene) = 0;

	static Window *create(const WindowProps &props = WindowProps());
};
} // namespace ale

#endif