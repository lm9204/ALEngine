#ifndef APP_H
#define APP_H

#include "Core/Base.h"
#include "Core/Window.h"
#include "Events/event.h"

namespace ale
{
class AL_API App
{
  public:
	App();
	virtual ~App();

	void run();

  private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
};

// To be defined in CLIENT
App *createApp();

} // namespace ale

#endif