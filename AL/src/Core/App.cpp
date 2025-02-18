#include "Core/App.h"
#include "ALpch.h"
#include <GLFW/glfw3.h>

#include "Scripting/ScriptingEngine.h"

namespace ale
{

App *App::s_Instance = nullptr;

App::App(const ApplicationSpecification &spec) : m_Spec(spec)
{
	// ASSERT s_Instance
	AL_CORE_INFO("App::App");
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::create());
	m_Window->setEventCallback(AL_BIND_EVENT_FN(App::onEvent));

	if (!m_Spec.m_WorkingDirectory.empty())
	{
		std::filesystem::current_path(m_Spec.m_WorkingDirectory);
	}

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
	ScriptingEngine::shutDown();
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
	while (m_Running)
	{
		// set delta time
		// float time = (float)glfwGetTime();
		auto time = std::chrono::high_resolution_clock::now();
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
	// dispatcher.dispatch<KeyPressedEvent>(AL_BIND_EVENT_FN(App::onWindowClose));
	dispatcher.dispatch<WindowResizeEvent>(AL_BIND_EVENT_FN(App::onWindowResize));

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

void App::close()
{
	m_Running = false;
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

bool App::onWindowClose(KeyPressedEvent &e)
{
	if (e.getKeyCode() == Key::Escape)
	{
		m_Running = false;
	}
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
	return false;
}

} // namespace ale