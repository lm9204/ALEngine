#include "EditorLayer.h"
#include "Scene/SceneSerializer.h"
#include "Utils/PlatformUtils.h"

namespace ale
{
EditorLayer::EditorLayer() : Layer("EditorLayer")
{
}

void EditorLayer::onAttach()
{
	// Editor에 필요한 texture들 create
	// Play, Stop, Pause etc.

	// scene 생성
	{
		// 아마 이런 흐름으로 작성
		{
			// Open Project File - 인자로 받아온 파일 경로
			// Script Engine Init
			// Open Scene File -> Parse Scene (Entity - Component)
		}
		// Temp - 지금 renderer 형식에 맞게
		m_Scene = Scene::createScene();
		m_SceneHierarchyPanel.setContext(m_Scene);

		// Entity 생성 - 적절한 Component를 Add 해야 함.
		auto box = m_Scene->createEntity("Plane");
		box.addComponent<ModelComponent>();
		box.getComponent<ModelComponent>().m_Model = Model::createPlaneModel();

		box.addComponent<TextureComponent>();
		box.getComponent<TextureComponent>().m_Texture = Texture::createTexture("textures/karina.jpg");
		box.getComponent<TransformComponent>().m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
		box.getComponent<TransformComponent>().m_Rotation = glm::vec3(0.0f, 0.0f, glm::radians(180.0f));
		box.getComponent<TransformComponent>().m_Scale = glm::vec3(5.0f * 0.74f, 5.0f, 1.0f);

		auto light = m_Scene->createEntity("Light");
		light.addComponent<ModelComponent>();
		light.getComponent<ModelComponent>().m_Model = Model::createSphereModel();

		light.addComponent<TextureComponent>();
		light.getComponent<TextureComponent>().m_Texture = Texture::createTexture("textures/texture.png");
		light.getComponent<TransformComponent>().m_Position = m_Scene->getLightPos();
		light.getComponent<TransformComponent>().m_Scale = glm::vec3(0.1f, 0.1f, 0.1f);

		Renderer &renderer = App::get().getRenderer();
		renderer.loadScene(m_Scene.get());
	}

	// camera setting
}

void EditorLayer::onDetach()
{
	// texture clean up
}

void EditorLayer::onUpdate(Timestep ts)
{
	// Scene의 State에 따라 update 구분
	// Edit, Play, ...
	m_CameraController.onUpdate(ts);
	m_Scene->onUpdate(ts);
}

void EditorLayer::onImGuiRender()
{
	// setDockingSpace();

	// Menu
	// setMenuBar();

	// Stats - hovered entity, rendered entities
	// viewport - texture descriptor set을 가져올 수 있는 방법 있으면 좋을듯
	// Drag & Drop
	// Gizmos

	m_SceneHierarchyPanel.onImGuiRender();

	SceneSerializer serializer(m_Scene);
	serializer.serialize("Sandbox/Project/Assets/Scenes/3DExample.ale");

	// ImGui::End(); // DockSpace -> ImGui::Begin, End 쌍 맞추기
}

void EditorLayer::onEvent(Event &e)
{
	m_CameraController.onEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseButtonPressedEvent>(AL_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
	dispatcher.dispatch<KeyPressedEvent>(AL_BIND_EVENT_FN(EditorLayer::onKeyPressed));
}

bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent &e)
{
	return false;
}

bool EditorLayer::onKeyPressed(KeyPressedEvent &e)
{
	if (e.isRepeat())
	{
		return false;
	}

	bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
	bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

	switch (e.getKeyCode())
	{
	case Key::N:
		if (control)
			; // new scene
		break;
	case Key::O:
		if (control)
			; // open scene
		break;
	case Key::S:
		if (control)
		{
			if (shift)
				; // save scene as
			else
				; // save scene
		}
		break;
	default:
		break;
	}

	return false;
}

void EditorLayer::setDockingSpace()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	bool p_open = true;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
						ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace", &p_open, window_flags); // begin dockspace

	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO &io = ImGui::GetIO();
	ImGuiStyle &style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	style.WindowMinSize.x = minWinSizeX;
}

void EditorLayer::setMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				;

			ImGui::Separator();

			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				;

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				;

			if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				;

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				App::get().close();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
				;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void EditorLayer::newProject()
{
}

void EditorLayer::openProject(const std::filesystem::path &path)
{
}

bool EditorLayer::openProject()
{
	std::string filepath = FileDialogs::openFile("AfterLife Project (*.hproj)\0*.hproj\0");
	if (filepath.empty())
		return false;

	openProject(filepath);
	return true;
}

void EditorLayer::saveProject()
{
}

void EditorLayer::newScene()
{
}

void EditorLayer::openScene()
{
	std::string filepath = FileDialogs::openFile("AfterLife Scene (*.ale)\0*.ale\0");
	if (!filepath.empty())
		openScene(filepath);
}

void EditorLayer::openScene(const std::filesystem::path &path)
{
	// 추후 수정 필요

	// if (m_SceneState != SceneState::Edit)
	// 	OnSceneStop();

	if (path.extension().string() != ".hazel")
	{
		AL_WARN("Could not load {0} - not a scene file", path.filename().string());
		return;
	}

	std::shared_ptr<Scene> newScene = Scene::createScene();
	SceneSerializer serializer(newScene);
	if (serializer.deserialize(path.string()))
	{
		// m_EditorScene = newScene;
		// m_SceneHierarchyPanel.setContext(m_EditorScene);

		// m_ActiveScene = m_EditorScene;
		// m_EditorScenePath = path;
	}
}

void EditorLayer::saveScene()
{
}

void EditorLayer::saveSceneAs()
{
}

} // namespace ale