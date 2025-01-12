#include "Platform/Windows/WindowsWindow.h"
#include "ALpch.h"
#include "Core/Log.h"
#include "Events/AppEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace ale
{
static bool s_GLFWinitialized = false;

static void GLFWErrorCallback(int error, const char *description)
{
	AL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

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

	AL_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

	if (!s_GLFWinitialized)
	{
		int32_t success = glfwInit();
		// ASSERT
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWinitialized = true;
	}

	m_Window = glfwCreateWindow((int32_t)props.width, (int32_t)props.height, m_Data.title.c_str(), nullptr, nullptr);
	// glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	// setVSync(true);

	// Set GLFW callbacks by lambda functions
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int32_t width, int32_t height) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;

		WindowResizeEvent event(width, height);
		data.eventCallback(event);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		data.eventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS: {
			KeyPressedEvent event(key, 0);
			data.eventCallback(event);
			break;
		}
		case GLFW_RELEASE: {
			KeyReleasedEvent event(key);
			data.eventCallback(event);
			break;
		}
		case GLFW_REPEAT: {
			KeyPressedEvent event(key, true);
			data.eventCallback(event);
			break;
		}
		}
	});

	glfwSetCharCallback(m_Window, [](GLFWwindow *window, uint32_t keycode) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

		KeyTypedEvent event(keycode);
		data.eventCallback(event);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		switch (action)
		{
		case GLFW_PRESS: {
			MouseButtonPressedEvent event(button);
			data.eventCallback(event);
			break;
		}
		case GLFW_RELEASE: {
			MouseButtonReleasedEvent event(button);
			data.eventCallback(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.eventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
		WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
		MouseMovedEvent event((float)xPos, (float)yPos);
		data.eventCallback(event);
		// data.scene->mouseMove(xPos, yPos);
	});

	AL_CORE_INFO("Init window end!");
}

void WindowsWindow::shutDown()
{
	glfwDestroyWindow(m_Window);
}

void WindowsWindow::onUpdate()
{
	glfwPollEvents();
	// glfwSwapBuffers(m_Window);
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