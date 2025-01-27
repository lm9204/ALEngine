#include "SceneHierarchyPanel.h"
#include "Scene/Component.h"

#include "Renderer/RenderingComponent.h"
#include "Scripting/ScriptingEngine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "UI/UI.h"

#include <glm/gtc/type_ptr.hpp>

#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace ale
{
SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene> &context)
{
	setContext(context);
}

void SceneHierarchyPanel::setContext(const std::shared_ptr<Scene> &context)
{
	m_Context = context;
	m_SelectionContext = {};
}

void SceneHierarchyPanel::onImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");
	if (m_Context)
	{
		// std::cout << "Scene Hierarchy\n";
		// m_Context->m_Registry.view<entt::entity>().each([&](auto entityID) {
		// 	Entity entity{entityID, m_Context.get()};
		// 	drawEntityNode(entity);
		// });

		auto view = m_Context->m_Registry.view<entt::entity>();

		for (auto it = view.begin(); it != view.end(); ++it)
		{
			Entity entity(*it, m_Context.get());
			drawEntityNode(entity);
		}

		// 왼쪽 클릭 && Hovered(마우스를 window에 올려뒀을 때)
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// 오른쪽 클릭으로 Popup 활성화
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty"))
				m_Context->createEntity("Empty");

			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Box"))
					;
				if (ImGui::MenuItem("Sphere"))
					;
				if (ImGui::MenuItem("Capsule"))
					;
				if (ImGui::MenuItem("Cylinder"))
					;
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	if (m_SelectionContext)
	{
		drawComponents(m_SelectionContext);
	}
	ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(Entity entity)
{
	auto &tag = entity.getComponent<TagComponent>().m_Tag;

	ImGuiTreeNodeFlags flags =
		((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth; // 선택 영역이 가장자리까지 넓어지게 설정
	bool opened = ImGui::TreeNodeEx((void *)(uint64_t)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void *)9817239, flags, tag.c_str());
		if (opened)
			ImGui::TreePop();
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_Context->destroyEntity(entity);
		if (m_SelectionContext == entity)
			m_SelectionContext = {};
	}
}

static void drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f,
							float columnWidth = 100.0f)
{
	ImGuiIO &io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template <typename T, typename UIFunction>
static void drawComponent(const std::string &name, Entity entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
											 ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
											 ImGuiTreeNodeFlags_FramePadding;
	if (entity.hasComponent<T>())
	{
		auto &component = entity.getComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();

		// 컴파일 타임에 타입 비교 수행
		if (!std::is_same<T, TransformComponent>::value)
		{
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.removeComponent<T>();
	}
}

void SceneHierarchyPanel::drawComponents(Entity entity)
{
	if (entity.hasComponent<TagComponent>())
	{
		auto &tag = entity.getComponent<TagComponent>().m_Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
		// ## 뒤의 text는 ImGui 내부의 ID로 사용.
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 200), ImVec2(600, 400));
	if (ImGui::BeginPopup("AddComponent"))
	{
		const char *text = "Component";
		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(text).x;
		float textPosX = (windowWidth - textWidth) * 0.5f;

		ImGui::SetCursorPosX(textPosX);
		ImGui::Text("%s", text);
		ImGui::Separator();

		displayAddComponentEntry<CameraComponent>("Camera");
		displayAddComponentEntry<ScriptComponent>("Script");
		displayAddComponentEntry<MeshRendererComponent>("Mesh Renderer");
		displayAddComponentEntry<LightComponent>("Light");
		// displayAddComponentEntry<RigidbodyComponent>("Rigidbody");
		// displayAddComponentEntry<BoxColliderComponent>("Box Collider");
		// displayAddComponentEntry<SphereColliderComponent>("Sphere Collider");
		// displayAddComponentEntry<CapsuleColliderComponent>("Capsule Collider");
		// displayAddComponentEntry<CylinderColliderComponent>("Cylinder Collider");

		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();

	drawComponent<TransformComponent>("Transform", entity, [](auto &component) {
		drawVec3Control("Position", component.m_Position);
		auto &rotation = glm::degrees(component.m_Rotation);
		drawVec3Control("Rotation", rotation);
		component.m_Rotation = glm::radians(rotation);
		drawVec3Control("Scale", component.m_Scale, 1.0f);
	});

	drawComponent<CameraComponent>("Camera", entity, [](auto &component) {
		auto &camera = component.m_Camera;

		float perspectiveVerticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
		if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
			camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

		float perspectiveNear = camera.getPerspectiveNearClip();
		if (ImGui::DragFloat("Near", &perspectiveNear))
			camera.setPerspectiveNearClip(perspectiveNear);

		float perspectiveFar = camera.getPerspectiveFarClip();
		if (ImGui::DragFloat("Far", &perspectiveFar))
			camera.setPerspectiveFarClip(perspectiveFar);
	});

	drawComponent<MeshRendererComponent>("MeshRenderer", entity, [entity, scene = m_Context](auto &component) mutable {
		uint32_t meshType = component.type;
		std::shared_ptr<RenderingComponent> rc = component.m_RenderingComponent;

		const char *meshTypeStrings[] = {"Box", "Sphere", "Plane", "None"};
		const char *currentMeshTypeString = meshTypeStrings[(int)meshType];

		if (ImGui::BeginCombo("Primitive Type", currentMeshTypeString))
		{
			for (int32_t i = 0; i < 4; ++i)
			{
				bool isSelected = currentMeshTypeString == meshTypeStrings[i];

				if (ImGui::Selectable(meshTypeStrings[i], isSelected))
				{
					currentMeshTypeString = meshTypeStrings[i];
					// model 정보 바꾸기

					std::shared_ptr<Material> mat = scene->getDefaultMaterial();
					component.type = i;
					if (i == 0)
					{
						component.m_RenderingComponent =
							RenderingComponent::createRenderingComponent(Model::createBoxModel(mat));
					}
					else if (i == 1)
					{
						component.m_RenderingComponent =
							RenderingComponent::createRenderingComponent(Model::createSphereModel(mat));
					}
					else if (i == 2)
					{
						component.m_RenderingComponent =
							RenderingComponent::createRenderingComponent(Model::createPlaneModel(mat));
					}
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	});

	drawComponent<LightComponent>("Light", entity, [](auto &component) {

	});

	drawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto &component) mutable {
		bool scriptClassExists = ScriptingEngine::entityClassExists(component.m_ClassName);

		UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

		static char buffer[64];
		strcpy(buffer, component.m_ClassName.c_str());

		if (ImGui::InputText("Class", buffer, sizeof(buffer)))
		{
			component.m_ClassName = buffer;
			return;
		}

		bool sceneRunning = scene->isRunning();

		if (sceneRunning)
		{
			std::shared_ptr<ScriptInstance> scriptInstance = ScriptingEngine::getEntityScriptInstance(entity.getUUID());
			if (scriptInstance)
			{
				const auto &fields = scriptInstance->getScriptClass()->getFields();
				for (const auto &[name, field] : fields)
				{
					if (field.m_Type == EScriptFieldType::FLOAT)
					{
						float data = scriptInstance->getFieldValue<float>(name);
						if (ImGui::DragFloat(name.c_str(), &data))
						{
							scriptInstance->setFieldValue(name, data);
						}
					}
				}
			}
		}
		else
		{
			if (scriptClassExists)
			{
				std::shared_ptr<ScriptClass> entityClass = ScriptingEngine::getEntityClass(component.m_ClassName);
				const auto &fields = entityClass->getFields();

				auto &entityFields = ScriptingEngine::getScriptFieldMap(entity);
				for (const auto &[name, field] : fields)
				{
					if (entityFields.find(name) != entityFields.end())
					{
						ScriptFieldInstance &scriptField = entityFields.at(name);

						if (field.m_Type == EScriptFieldType::FLOAT)
						{
							float data = scriptField.getValue<float>();
							if (ImGui::DragFloat(name.c_str(), &data))
								scriptField.setValue<float>(data);
						}
					}
					else
					{
						if (field.m_Type == EScriptFieldType::FLOAT)
						{
							float data = 0.0f;
							if (ImGui::DragFloat(name.c_str(), &data))
							{
								ScriptFieldInstance &fieldInstance = entityFields[name];
								fieldInstance.m_Field = field;
								fieldInstance.setValue<float>(data);
							}
						}
					}
				}
			}
		}
	});
}

template <typename T> void SceneHierarchyPanel::displayAddComponentEntry(const std::string &entryName)
{
	if (!m_SelectionContext.hasComponent<T>())
	{
		if (ImGui::Selectable(entryName.c_str()))
		{
			m_SelectionContext.addComponent<T>();
			ImGui::CloseCurrentPopup();
		}
	}
}
} // namespace ale