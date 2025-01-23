#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include "AL.h"
#include "Panel/ContentBrowserPanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Renderer/CameraController.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/Texture.h"

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
	bool onResized(WindowResizeEvent &e);

	// GUI
	void setDockingSpace();
	void setMenuBar();
	void uiToolBar();

	// PROJECT
	void newProject();
	bool openProject(const std::filesystem::path &path);
	bool openProject();
	void saveProject();

	// SCENE
	void newScene();
	void openScene();
	void openScene(const std::filesystem::path &path);
	void saveScene();
	void saveSceneAs();

	void serializeScene(std::shared_ptr<Scene> &scene, const std::filesystem::path &path);

	void onScenePlay();
	void onScenePause();
	void onSceneStop();

	void loadSceneToRenderer(std::shared_ptr<Scene> &scene);

  private:
	CameraController m_CameraController;
	EditorCamera m_EditorCamera;

	std::shared_ptr<Scene> m_EditorScene;
	std::shared_ptr<Scene> m_ActiveScene;

	std::filesystem::path m_EditorScenePath;

	SceneHierarchyPanel m_SceneHierarchyPanel;
	std::unique_ptr<ContentBrowserPanel> m_ContentBrowserPanel;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDevice device;

	glm::vec2 m_ViewportSize = {0.0f, 0.0f};

	enum class ESceneState
	{
		EDIT = 0,
		PLAY
	};

	ESceneState m_SceneState = ESceneState::EDIT;
	std::shared_ptr<Texture> m_PlayIcon, m_PauseIcon, m_StepIcon;
	ImTextureID playTextureID, pauseTextureID, stepTextureID;
};

} // namespace ale

#endif