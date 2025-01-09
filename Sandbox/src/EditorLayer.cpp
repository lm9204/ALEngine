#include "EditorLayer.h"

namespace ale
{
EditorLayer::EditorLayer() : Layer("EditorLayer")
{
}

void EditorLayer::onAttach()
{
}

void EditorLayer::onDetach()
{
}

void EditorLayer::onUpdate(Timestep ts)
{
	m_CameraController.onUpdate(ts);
}

void EditorLayer::onImGuiRender()
{
	setDockingSpace();

	// viewport
	// texture descriptor set을 가져올 수 있는 방법 있으면 좋을듯
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
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

// Hierarchy 창
void EditorLayer::showHierarchyWindow()
{
	if (ImGui::CollapsingHeader("Root Object"))
	{
		ImGui::Text("Child Object 1");
		ImGui::Text("Child Object 2");
	}
}

// Scene 창
void EditorLayer::showSceneWindow()
{
	ImGui::Text("Scene View");
	ImGui::Text("Render the 2D/3D scene here.");
	// 여기에 2D/3D 렌더링 코드를 추가
}

// Inspector 창
void EditorLayer::showInspectorWindow()
{
	ImGui::Text("Inspector");
	static float position[3] = {0.0f, 0.0f, 0.0f};
	ImGui::InputFloat3("Position", position);
	static float rotation[3] = {0.0f, 0.0f, 0.0f};
	ImGui::InputFloat3("Rotation", rotation);
	static float scale[3] = {1.0f, 1.0f, 1.0f};
	ImGui::InputFloat3("Scale", scale);
}

// Project 창
void EditorLayer::showProjectWindow()
{
	ImGui::Text("Project Files");
	if (ImGui::TreeNode("Assets"))
	{
		ImGui::Text("Material.mat");
		ImGui::Text("Model.fbx");
		ImGui::Text("Script.cs");
		ImGui::TreePop();
	}
}

} // namespace ale