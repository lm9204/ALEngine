#include "Core/App.h"
#include "ALpch.h"
#include <GLFW/glfw3.h>

namespace ale
{

App::App()
{
	m_Window = std::unique_ptr<Window>(Window::create());
	m_Window->setEventCallback(AL_BIND_EVENT_FN(App::onEvent));
}

App::~App()
{
}

void App::run()
{
	while (m_Running)
	{
		m_Window->onUpdate();
	}
}

void App::onEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>(AL_BIND_EVENT_FN(App::onWindowClose));
	// dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(App::onWindowResize));

	AL_CORE_INFO("{0}", e.toString());
}

bool App::onWindowClose(WindowCloseEvent &e)
{
	m_Running = false;
	return true;
}

bool App::onWindowResize(WindowResizeEvent &e)
{
	if (e.getWidth() == 0 || e.getHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	// renderer resize
	return false;
}

} // namespace ale