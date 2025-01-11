#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"

#include "Core/App.h"

namespace ale
{
Scene::~Scene()
{
}

std::unique_ptr<Scene> Scene::createScene()
{
	std::unique_ptr<Scene> scene = std::unique_ptr<Scene>(new Scene());
	// scene->initScene();
	return scene;
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
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = {e, this};
				// update entity
			}
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto &nsc) {
				if (!nsc.instance)
				{
					nsc.instance = nsc.instantiateScript();
					nsc.instance->m_Entity = Entity{entity, this};
					nsc.instance->onCreate();
				}
				nsc.instance->onUpdate(ts);
			});
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
		renderScene();
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

void Scene::renderScene()
{
	Renderer &renderer = App::get().getRenderer();
	// Draw Models
	renderer.drawFrame(this);
}

} // namespace ale