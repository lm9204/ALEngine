#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"

namespace ale
{
Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::createEntity(const std::string &name)
{
	Entity entity = {m_Registry.create(), this};
	entity.addComponent<TransformComponent>();
	auto &tag = entity.addComponent<TagComponent>();
	tag.m_Tag = name.empty() ? "Entity" : name;

	return entity;
}

void Scene::onUpdate(Timestep ts)
{
	// update needs to be classified by states
	// Edit, Play, (Simulate??), Pause
	// if Update runtime(maybe play button)
	{
		// update scripts
		{
		}
		// Do Physics
		{
		}
	}

	// find main camera
	{
	}

	// if main Camera exists
	{
		// Renderer draw frame
	}
}

void Scene::onViewportResize(uint32_t width, uint32_t height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	// Resize non Fixed Aspect ratio camera
	auto view = m_Registry.view<CamerComponent>();
	for (auto entity : view)
	{
		auto &cameraComponent = view.get<CamerComponent>(entity);
		if (!cameraComponent.m_FixedAspectRatio)
			cameraComponent.m_Camera.setViewportSize(width, height); // set viewport size
	}
}

} // namespace ale