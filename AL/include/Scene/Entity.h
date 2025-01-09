#ifndef ENTITY_H
#define ENTITY_H

#include "Scene/Scene.h"

namespace ale
{
class Entity
{
  public:
	Entity() = default;
	Entity(entt::entity handle, Scene *scene);
	Entity(const Entity &other) = default;

	template <typename T, typename... Args> T &addComponent(Args &&...args)
	{
		// ASSERT NOT HAS COMPONENT
		return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template <typename T, typename... Args> T &addOrReplaceComponent(Args &&...args)
	{
		// ASSERT NOT HAS COMPONENT
		return m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template <typename T> T &getComponent()
	{
		// ASSERT HAS COMPONENT
		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template <typename T> void removeComponent()
	{
		// ASSERT HAS COMPONENT
		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	template <typename T> bool hasComponent()
	{
		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

  private:
	entt::entity m_EntityHandle{entt::null};
	Scene *m_Scene = nullptr;
};
} // namespace ale

#endif