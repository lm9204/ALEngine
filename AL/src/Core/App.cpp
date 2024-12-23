#include "Core/App.h"
#include "ALpch.h"
#include <GLFW/glfw3.h>

namespace ale
{
App::App()
{
	m_Window = std::unique_ptr<Window>(Window::create());
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
} // namespace ale