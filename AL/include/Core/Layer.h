#ifndef LAYER_H
#define LAYER_H

#include "Core/Base.h"
#include "Events/Event.h"

namespace ale
{
class AL_API Layer
{
  public:
	Layer(const std::string &name = "Layer");
	virtual ~Layer() = default;

	virtual void onAttach();
	virtual void onDetach();
	virtual void onUpdate();
	virtual void onImGuiRender();
	virtual void onEvent(Event &event);


	const std::string &getName() const
	{
		return m_DebugName;
	}

  private:
	std::string m_DebugName;
};
} // namespace ale

#endif