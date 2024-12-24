#ifndef APP_H
#define APP_H

#include "Core/Base.h"
#include "Core/Window.h"
#include "Events/AppEvent.h"
#include "Events/Event.h"

namespace ale
{
class AL_API App
{
  public:
	App();
	virtual ~App();

	void run();
	void onEvent(Event &e);

  private:
	bool onWindowClose(WindowCloseEvent &e);
	bool onWindowResize(WindowResizeEvent &e);

	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
	bool m_Minimized = false;
};

// To be defined in CLIENT
App *createApp();

} // namespace ale

#endif