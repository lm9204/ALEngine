#include "Platform/WindowsWindow.h"
#include "ALpch.h"
#include "Core/Log.h"

namespace ale
{
static bool s_GLFWinitialized = false;

Window *Window::create(const WindowProps &props)
{
	return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps &props)
{
	init(props);
}

WindowsWindow::~WindowsWindow()
{
	shutDown();
}

void WindowsWindow::init(const WindowProps &props)
{
	m_Data.title = props.title;
	m_Data.width = props.width;
	m_Data.height = props.height;

	AL_CORE_INFO("Createing window {0} ({1}, {2})", props.title, props.width, props.height);

	if (!s_GLFWinitialized)
	{
		int32_t success = glfwInit();
		// ASSERT
		s_GLFWinitialized = true;
	}

	m_Window = glfwCreateWindow((int32_t)props.width, (int32_t)props.height, m_Data.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	setVSync(true);
}

void WindowsWindow::shutDown()
{
	glfwDestroyWindow(m_Window);
}

void WindowsWindow::onUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void WindowsWindow::setVSync(bool enabled)
{
	if (enabled)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}
	m_Data.vSync = enabled;
}

bool WindowsWindow::isVSync() const
{
	return m_Data.vSync;
}
} // namespace ale