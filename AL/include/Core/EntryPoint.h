#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Core/App.h"
#include "Core/Base.h"

#ifdef AL_PLATFORM_WINDOWS

extern ale::App *ale::createApp();

int main(int argc, char **argv)
{
	ale::Log::init();
	AL_CORE_WARN("Initialized Log!");
	AL_INFO("Hello!");

	auto app = ale::createApp();
	app->run();
	delete app;
}
#endif

#endif