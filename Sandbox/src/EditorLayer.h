#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include "AL.h"
#include "Panel/SceneHierarchyPanel.h"
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
	void setMenuBar();

  private:
	CameraController m_CameraController;
	std::shared_ptr<Scene> m_Scene;
	SceneHierarchyPanel m_SceneHierarchyPanel;
};

} // namespace ale

#endif