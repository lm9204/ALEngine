#include "AL.h"
#include "Core/EntryPoint.h"

class Sandbox : public ale::App
{
  public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

ale::App *ale::createApp()
{
	return new Sandbox();
}