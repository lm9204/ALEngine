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
class App
{
  public:
	App();
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
	static App &get();

  private:
	bool onWindowClose(WindowCloseEvent &e);
	bool onWindowClose(KeyPressedEvent &e);
	bool onWindowResize(WindowResizeEvent &e);

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Renderer> m_Renderer;
	ImGuiLayer *m_ImGuiLayer;

	LayerStack m_LayerStack;

	float m_LastFrameTime = 0.0f;
	bool m_Running = true;
	bool m_Minimized = false;

  private:
	static App *s_Instance;
};

// To be defined in CLIENT
App *createApp();

} // namespace ale

#endif