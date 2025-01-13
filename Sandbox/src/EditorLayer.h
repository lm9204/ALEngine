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

  private:
	// EVENTS
	bool onMouseButtonPressed(MouseButtonPressedEvent &e);
	bool onKeyPressed(KeyPressedEvent &e);

	// GUI
	void setDockingSpace();
	void setMenuBar();

	// PROJECT
	void newProject();
	void openProject(const std::filesystem::path &path);
	bool openProject();
	void saveProject();

	// SCENE
	void newScene();
	void openScene();
	void openScene(const std::filesystem::path &path);
	void saveScene();
	void saveSceneAs();

  private:
	CameraController m_CameraController;
	std::shared_ptr<Scene> m_Scene;
	SceneHierarchyPanel m_SceneHierarchyPanel;
};

} // namespace ale

#endif