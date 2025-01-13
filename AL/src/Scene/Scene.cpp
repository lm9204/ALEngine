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

std::shared_ptr<Scene> Scene::createScene()
{
	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
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

void Scene::destroyEntity(Entity entity)
{
	m_Registry.destroy(entity);
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
	auto view = m_Registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto &cameraComponent = view.get<CameraComponent>(entity);
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

// 컴파일 타임에 조건 확인
template <typename T> void Scene::onComponentAdded(Entity entity, T &component)
{
	static_assert(sizeof(T) == 0);
}

template <> void Scene::onComponentAdded(Entity entity, TagComponent &component)
{
}

template <> void Scene::onComponentAdded(Entity entity, TransformComponent &component)
{
}

template <> void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent &component)
{
	if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
		component.m_Camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
}

template <> void Scene::onComponentAdded<MeshRendererComponent>(Entity entity, MeshRendererComponent &component)
{
}

template <> void Scene::onComponentAdded<ModelComponent>(Entity entity, ModelComponent &component)
{
}

template <> void Scene::onComponentAdded<TextureComponent>(Entity entity, TextureComponent &component)
{
}

template <> void Scene::onComponentAdded<LightComponent>(Entity entity, LightComponent &component)
{
}

template <> void Scene::onComponentAdded<RigidbodyComponent>(Entity entity, RigidbodyComponent &component)
{
}

template <> void Scene::onComponentAdded<BoxColliderComponent>(Entity entity, BoxColliderComponent &component)
{
}

template <> void Scene::onComponentAdded<SphereColliderComponent>(Entity entity, SphereColliderComponent &component)
{
}

template <> void Scene::onComponentAdded<CapsuleColliderComponent>(Entity entity, CapsuleColliderComponent &component)
{
}

template <> void Scene::onComponentAdded<CylinderColliderComponent>(Entity entity, CylinderColliderComponent &component)
{
}

template <> void Scene::onComponentAdded<ScriptComponent>(Entity entity, ScriptComponent &component)
{
}
} // namespace ale