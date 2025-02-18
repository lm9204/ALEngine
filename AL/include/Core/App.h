#ifndef APP_H
#define APP_H

#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Core/Window.h"

#include "ImGui/ImGuiLayer.h"

#include "Events/AppEvent.h"
#include "Events/Event.h"

#include "Renderer/Common.h"
#include "Renderer/Renderer.h"

namespace ale
{
struct ApplicationCommandLineArgs
{
	int count = 0;
	char **args = nullptr;

	const char *operator[](int index) const
	{
		return args[index];
	}
};

struct ApplicationSpecification
{
	std::string m_Name = "ALEngine";
	std::string m_WorkingDirectory;
	ApplicationCommandLineArgs m_CommandLineArgs;
};

class App
{
  public:
	App(const ApplicationSpecification &spec);
	virtual ~App();

	void run();
	void onEvent(Event &e);
	void cleanup();
	void close();

	void pushLayer(Layer *layer);
	void pushOverlay(Layer *layer);

	Window &getWindow()
	{
		return *m_Window;
	}
	Renderer &getRenderer()
	{
		return *m_Renderer;
	}

	const ApplicationSpecification &getSpecification() const
	{
		return m_Spec;
	}

	static App &get();

  private:
	bool onWindowClose(WindowCloseEvent &e);
	bool onWindowClose(KeyPressedEvent &e);
	bool onWindowResize(WindowResizeEvent &e);

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Renderer> m_Renderer;
	ImGuiLayer *m_ImGuiLayer;

	LayerStack m_LayerStack;

	ApplicationSpecification m_Spec;

	Chrono::TimePoint m_LastFrameTime;
	bool m_Running = true;
	bool m_Minimized = false;

  private:
	static App *s_Instance;
};

// To be defined in CLIENT
App *createApp(ApplicationCommandLineArgs args);

} // namespace ale

#endif