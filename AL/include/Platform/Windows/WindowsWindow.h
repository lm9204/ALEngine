#ifndef WINDOWSWINDOW_H
#define WINDOWSWINDOW_H

#include "Core/Window.h"
#include <GLFW/glfw3.h>
#include "Renderer/Scene.h"

namespace ale
{
// Window derived class
// implemented by GLFW - used in Windows platform
class WindowsWindow : public Window
{
  public:
	WindowsWindow(const WindowProps &props);
	virtual ~WindowsWindow();

	void onUpdate() override;

	inline uint32_t getWidth() const override
	{
		return m_Data.width;
	}

	inline uint32_t getHeight() const override
	{
		return m_Data.height;
	}

	inline void setEventCallback(const EventCallbackFn &callback) override
	{
		m_Data.eventCallback = callback;
	}

	inline GLFWwindow *getNativeWindow() const override
	{
		return m_Window;
	}

	void setVSync(bool enabled) override;
	bool isVSync() const override;
	void bindScene(Scene *scene) override;

  private:
	virtual void init(const WindowProps &props);
	virtual void shutDown();

  private:
	GLFWwindow *m_Window;

	struct WindowData
	{
		std::string title;
		uint32_t width, height;
		bool vSync;
		Scene* scene;

		EventCallbackFn eventCallback;
	};

	WindowData m_Data;
};
} // namespace ale

#endif