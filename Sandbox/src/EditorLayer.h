#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include "AL.h"
#include "Renderer/CameraController.h"

namespace ale
{
class EditorLayer : public Layer
{
  public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(Timestep ts) override;
	void onImGuiRender() override;
	void onEvent(Event &e) override;

	void setDockingSpace();

  private:
	CameraController m_CameraController;
	std::unique_ptr<Scene> m_Scene;
};

} // namespace ale

#endif