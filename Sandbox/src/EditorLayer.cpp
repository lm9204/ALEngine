#include "EditorLayer.h"

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

	// ImGui::End(); // DockSpace -> ImGui::Begin, End 쌍 맞추기
}

void EditorLayer::onEvent(Event &e)
{
	m_CameraController.onEvent(e);
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
} // namespace ale