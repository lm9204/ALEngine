#include "Core/App.h"
#include "ALpch.h"
#include <GLFW/glfw3.h>

namespace ale
{

App::App()
{
	m_Window = std::unique_ptr<Window>(Window::create());
	m_Window->setEventCallback(AL_BIND_EVENT_FN(App::onEvent));
	renderer = Renderer::createRenderer(m_Window->getWindow());
	scene = Scene::createScene();
	renderer->loadScene(scene.get());

	// init renderer
}

App::~App()
{
}

void App::pushLayer(Layer *layer)
{
	m_LayerStack.pushLayer(layer);
	// layer->onAttach();
}

void App::pushOverlay(Layer *layer)
{
	m_LayerStack.pushOverlay(layer);
	// layer->onAttach();
}

void App::run()
{
	AL_CORE_INFO("App::run");
	while (m_Running)
	{
		for (Layer *layer : m_LayerStack)
		{
			layer->onUpdate();
		}
		m_Window->onUpdate();
		renderer->drawFrame(scene.get());
	}
	vkDeviceWaitIdle(renderer->getDevice());
}

void App::onEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>(AL_BIND_EVENT_FN(App::onWindowClose));
	// dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(App::onWindowResize));

	AL_CORE_INFO("{0}", e.toString());

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if (e.m_Handled)
			break;
		(*it)->onEvent(e);
	}
}

void App::cleanup()
{
	scene->cleanup();
	renderer->cleanup();
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