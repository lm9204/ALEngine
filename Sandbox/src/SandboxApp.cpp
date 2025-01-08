#include "AL.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"

namespace ale
{
class Sandbox : public App
{
  public:
	Sandbox()
	{
		pushLayer(new EditorLayer());
	}

	~Sandbox()
	{
	}
};

App *createApp()
{
	return new Sandbox();
}

} // namespace ale
