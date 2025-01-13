#pragma once

#include "entt.hpp"

#include "Core/Timestep.h"
#include <glm/glm.hpp>

namespace ale
{
class Entity;

class Scene
{
  public:
	Scene() = default;
	~Scene();

	static std::shared_ptr<Scene> createScene();
	Entity createEntity(const std::string &name = "");
	void destroyEntity(Entity entity);

	void onUpdate(Timestep ts);
	void onViewportResize(uint32_t width, uint32_t height);

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

	void renderScene();

  private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	glm::vec3 m_lightPos{0.0f, 1.0f, 0.0f};

	friend class Entity;
	friend class SceneSerializer;
	friend class SceneHierarchyPanel;
};
} // namespace ale
