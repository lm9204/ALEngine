#ifndef SCENEHIERARCHYPANEL_H
#define SCENEHIERARCHYPANEL_H

#include "Core/Base.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace ale
{
class SceneHierarchyPanel
{
  public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const std::shared_ptr<Scene> &context);

	void setContext(const std::shared_ptr<Scene> &context);
	void onImGuiRender();

	Entity getSelectedEntity() const
	{
		return m_SelectionContext;
	}

	void setSelectedEntity(Entity entity);

  private:
	template <typename T> void displayAddComponentEntry(const std::string &entryName);

	void drawEntityNode(Entity entity);
	void drawComponents(Entity entity);

	void updateActiveInfo(Entity &entity, bool parentEffectiveActive);
	void updateRelationship(Entity &newParent, Entity &child);
	void updateRelationship(Entity &entity);
	void updateTransforms(Entity entity);
	void updateTransformRecursive(Entity entity, const glm::mat4 &parentWorldTransform);

  private:
	std::shared_ptr<Scene> m_Context;
	Entity m_SelectionContext;
};
} // namespace ale

#endif