#ifndef ENTITY_H
#define ENTITY_H

#include "Scene/Component.h"
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
		T &component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		m_Scene->onComponentAdded<T>(*this, component);
		return component;
	}

	template <typename T, typename... Args> T &addOrReplaceComponent(Args &&...args)
	{
		// ASSERT NOT HAS COMPONENT
		T &component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		m_Scene->onComponentAdded<T>(*this, component);
		return component;
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

	// 템플릿 완전 특수화
	template <> void removeComponent<MeshRendererComponent>()
	{
		auto &component = m_Scene->getComponent<MeshRendererComponent>(m_EntityHandle);
		m_Scene->removeEntityInCullTree(component.nodeId);
		m_Scene->m_Registry.remove<MeshRendererComponent>(m_EntityHandle);
	}

	template <typename T> bool hasComponent()
	{
		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

	UUID getUUID()
	{
		return getComponent<IDComponent>().m_ID;
	}
	const std::string &getTag()
	{
		return getComponent<TagComponent>().m_Tag;
	}

	operator bool() const
	{
		return m_EntityHandle != entt::null;
	}
	operator entt::entity() const
	{
		return m_EntityHandle;
	}
	operator uint32_t() const
	{
		return (uint32_t)m_EntityHandle;
	}

	bool operator==(const Entity &other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity &other) const
	{
		return !(*this == other);
	}

  private:
	entt::entity m_EntityHandle{entt::null};
	Scene *m_Scene = nullptr;
};
} // namespace ale

#endif