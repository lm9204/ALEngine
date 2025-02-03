#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "Core/App.h"
#include "Core/Base.h"

#ifdef AL_PLATFORM_WINDOWS

extern ale::App *ale::createApp(ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
	ale::Log::init();

	try
	{
		if (argc == 1)
		{
			argc = 2;
			argv[1] = "./Sandbox/Project/AfterLife.alproj";
		}
		auto app = ale::createApp({argc, argv});
		app->run();
		delete app;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
#endif

#endif