#pragma once

#include "entt.hpp"

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include <glm/glm.hpp>

#include "Renderer/EditorCamera.h"

namespace ale
{
class Entity;

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

	void onUpdateEditor(EditorCamera &camera);
	void onUpdateRuntime(Timestep ts);
	void onViewportResize(uint32_t width, uint32_t height);

	void step(int32_t frames);

	void setPaused(bool pause)
	{
		m_IsPaused = pause;
	}

	glm::vec3 &getLightPos()
	{
		return m_lightPos;
	}

	template <typename... Components> auto getAllEntitiesWith()
	{
		return m_Registry.view<Components...>();
	}

  private:
	template <typename T> void onComponentAdded(Entity entity, T &component);

	void renderScene(EditorCamera &camera);

  private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	glm::vec3 m_lightPos{0.0f, 1.0f, 0.0f};
	bool m_IsPaused = false;
	int32_t m_StepFrames = 0;

	std::unordered_map<UUID, entt::entity> m_EntityMap;

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
};
} // namespace ale
