#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Core/Layer.h"

namespace ale
{
class AL_API ImGuiLayer : public Layer
{
  public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;
	void onEvent(Event &event) override;

  private:
};
} // namespace ale

#endif