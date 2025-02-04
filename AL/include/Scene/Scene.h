#pragma once

#include "entt.hpp"

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include <glm/glm.hpp>

#include "Renderer/EditorCamera.h"
#include "Renderer/Material.h"

namespace ale
{
class Entity;
class Model;
class World;

class Scene
{
  public:
	Scene() = default;
	~Scene();

	static std::shared_ptr<Scene> copyScene(std::shared_ptr<Scene> scene);

	static std::shared_ptr<Scene> createScene();
	Entity createEntity(const std::string &name = "");
	Entity createEntityWithUUID(UUID uuid, const std::string &name = "");
	void destroyEntity(Entity entity);

	void onRuntimeStart();
	void onRuntimeStop();

	void onUpdateEditor(EditorCamera &camera);
	void onUpdateRuntime(Timestep ts);
	void onViewportResize(uint32_t width, uint32_t height);

	void step(int32_t frames);

	void setPaused(bool pause)
	{
		m_IsPaused = pause;
	}

	bool isRunning() const
	{
		return m_IsRunning;
	}

	glm::vec3 &getLightPos()
	{
		return m_lightPos;
	}
	glm::vec3 &getCamPos()
	{
		return m_CameraPos;
	}
	float &getAmbientStrength()
	{
		return m_ambientStrength;
	}
	std::shared_ptr<Material> &getDefaultMaterial()
	{
		return m_defaultMaterial;
	}
	std::shared_ptr<Model> getDefaultModel(int32_t idx);

	Entity findEntityByName(std::string_view name);
	Entity getEntityByUUID(UUID uuid);

	template <typename... Components> auto getAllEntitiesWith()
	{
		return m_Registry.view<Components...>();
	}

  private:
	template <typename T> void onComponentAdded(Entity entity, T &component);

	void initScene();
	void renderScene(EditorCamera &camera);
	void onPhysicsStart();
	void onPhysicsStop();

	void setCamPos(glm::vec3 &pos)
	{
		m_CameraPos = pos;
	}

  private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	glm::vec3 m_lightPos{0.0f, 1.0f, 0.0f};
	glm::vec3 m_CameraPos{0.0f, 0.0f, 10.0f};
	bool m_IsPaused = false;
	bool m_IsRunning = false;
	int32_t m_StepFrames = 0;

	std::unordered_map<UUID, entt::entity> m_EntityMap;

	DefaultTextures m_defaultTextures;
	std::shared_ptr<Material> m_defaultMaterial;
	std::shared_ptr<Model> m_boxModel;
	std::shared_ptr<Model> m_sphereModel;
	std::shared_ptr<Model> m_planeModel;

	World *m_World = nullptr;

	float m_ambientStrength{0.1f};

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
};
} // namespace ale
