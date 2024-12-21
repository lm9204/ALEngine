#include "AL.h"
#include "ALpch.h"

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