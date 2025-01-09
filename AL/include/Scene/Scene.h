#ifndef SCENE_H
#define SCENE_H

#include "entt.hpp"

#include "Core/Timestep.h"

namespace ale
{
class Scene
{
  public:
	Scene();
	~Scene();

	Entity createEntity(const std::string &name = "");

	void onUpdate(Timestep ts);
	void onViewportResize(uint32_t width, uint32_t height);

  private:
	entt::registry m_Registry;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	friend class Entity;
};
} // namespace ale

#endif