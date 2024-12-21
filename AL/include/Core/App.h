#ifndef APP_H
#define APP_H

#include "ALpch.h"
#include "Core/Base.h"

namespace ale
{
class AL_API App
{
  public:
	App();
	virtual ~App();

	void run();
};

// To be defined in CLIENT
App *createApp();

} // namespace ale

#endif