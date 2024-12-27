#ifndef APP_H
#define APP_H

#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Events/AppEvent.h"
#include "Events/Event.h"
#include "Renderer/Common.h"
#include "Renderer/Renderer.h"
#include "Renderer/Scene.h"

namespace ale
{
class AL_API App
{
  public:
	App();
	virtual ~App();

	void run();
	void onEvent(Event &e);
	void cleanup();

	void pushLayer(Layer *layer);
	void pushOverlay(Layer *layer);

  private:
	bool onWindowClose(WindowCloseEvent &e);
	bool onWindowResize(WindowResizeEvent &e);

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Scene> scene;

	LayerStack m_LayerStack;

	bool m_Running = true;
	bool m_Minimized = false;
};

// To be defined in CLIENT
App *createApp();

} // namespace ale

#endif