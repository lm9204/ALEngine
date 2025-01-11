#include "Core/App.h"
#include "ALpch.h"
#include <GLFW/glfw3.h>

namespace ale
{

App *App::s_Instance = nullptr;

App::App()
{
	// ASSERT s_Instance
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::create());
	m_Window->setEventCallback(AL_BIND_EVENT_FN(App::onEvent));

	// init renderer
	m_Renderer = Renderer::createRenderer(m_Window->getNativeWindow());
	// m_Scene = Scene::createScene();
	// m_Renderer->loadScene(m_Scene.get());

	// ImGuiLayer created
	m_ImGuiLayer = new ImGuiLayer();
	pushOverlay(m_ImGuiLayer);
}

App::~App()
{
}

void App::pushLayer(Layer *layer)
{
	m_LayerStack.pushLayer(layer);
	layer->onAttach();
}

void App::pushOverlay(Layer *layer)
{
	m_LayerStack.pushOverlay(layer);
	layer->onAttach();
}

void App::run()
{
	AL_CORE_INFO("App::run");
	while (m_Running && !glfwWindowShouldClose(m_Window->getNativeWindow()))
	{
		// set delta time
		float time = (float)glfwGetTime();
		Timestep ts = time - m_LastFrameTime;
		m_LastFrameTime = time;
		// AL_CORE_TRACE("Delta time: {0}s ({1}ms))", ts.getSeconds(), ts.getMiliSeconds());

		// layer stack update
		m_ImGuiLayer->beginFrame();

		// layer stack ImGuiRender
		for (Layer *layer : m_LayerStack)
		{
			layer->onImGuiRender();
		}

		for (Layer *layer : m_LayerStack)
		{
			layer->onUpdate(ts);
		}
		m_Window->onUpdate();
	}
	vkDeviceWaitIdle(m_Renderer->getDevice());
}

void App::onEvent(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatch<WindowCloseEvent>(AL_BIND_EVENT_FN(App::onWindowClose));
	// dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(App::onWindowResize));

	// AL_CORE_INFO("{0}", e.toString());

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if (e.m_Handled)
			break;
		(*it)->onEvent(e);
	}
}

void App::cleanup()
{
	// m_Scene->cleanup();
	m_Renderer->cleanup();
}

App &App::get()
{
	return *s_Instance;
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