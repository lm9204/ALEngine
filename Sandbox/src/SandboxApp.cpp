#include "AL.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"

namespace ale
{
class Sandbox : public App
{
  public:
	Sandbox(const ApplicationSpecification &spec) : App(spec)
	{
		pushLayer(new EditorLayer());
	}

	~Sandbox()
	{
	}
};

App *createApp(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.m_Name = "ALEngine";
	spec.m_CommandLineArgs = args;

	return new Sandbox(spec);
}

} // namespace ale
