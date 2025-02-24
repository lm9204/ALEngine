#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/UUID.h"

#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include "Renderer/SAComponent.h"

#include "Scene/SceneCamera.h"

#include "Scene/entt.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ale
{
struct IDComponent
{
	UUID m_ID;

	IDComponent() = default;
	IDComponent(const IDComponent &) = default;
};

struct TagComponent
{
	std::string m_Tag;
	bool m_isActive = true;	  // 에디터 상 활성 여부
	bool m_selfActive = true; // 부모와 관계없이 자신의 활성 여부

	TagComponent() = default;
	TagComponent(const TagComponent &) = default;
	TagComponent(const std::string &tag) : m_Tag(tag)
	{
	}
};

struct TransformComponent
{
	// 속성
	glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_Scale = {1.0f, 1.0f, 1.0f};
	glm::vec3 m_LastPosition = {0.0f, 0.0f, 0.0f};

	bool m_isMoved = false;

	glm::mat4 m_WorldTransform = glm::mat4(1.0f);

	// 생성자
	TransformComponent() = default;
	TransformComponent(const TransformComponent &) = default;
	TransformComponent(const glm::vec3 &position) : m_Position(position), m_LastPosition(position)
	{
	}

	glm::mat4 getTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

		return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
	}

	float getMaxScale()
	{
		return std::max(m_Scale.x, std::max(m_Scale.y, m_Scale.z));
	}
};

struct RelationshipComponent
{
	entt::entity parent = entt::null;
	std::vector<entt::entity> children;

	RelationshipComponent() = default;
	RelationshipComponent(const RelationshipComponent &) = default;
};

// RENDERER
// Mesh Renderer - Cube, Sphere, Cylinder, Capsule

class RenderingComponent;

struct MeshRendererComponent
{
	std::shared_ptr<RenderingComponent> m_RenderingComponent;
	uint32_t type;
	std::string path = "";
	std::string matPath = "";
	bool isMatChanged = false;

	// Culling
	int32_t nodeId = NULL_NODE;
	CullSphere cullSphere;
	ECullState cullState;

	MeshRendererComponent() = default;
	MeshRendererComponent(const MeshRendererComponent &) = default;
};

struct ModelComponent
{
	std::shared_ptr<Model> m_Model;

	ModelComponent() = default;
	ModelComponent(const ModelComponent &) = default;
};

struct SkeletalAnimatorComponent
{
	std::shared_ptr<SAComponent> sac;

	bool m_IsPlaying = false;
	bool m_IsTimelineDrag = false;
	bool m_IsChanged = false;
	float m_SpeedFactor = 1.0f;
	std::vector<bool> m_Repeats;
	std::map<std::string, std::function<bool()>> m_Methods;



	SkeletalAnimatorComponent() = default;
	SkeletalAnimatorComponent(const SkeletalAnimatorComponent &) = default;
};

struct TextureComponent
{
	std::shared_ptr<Texture> m_Texture;

	TextureComponent() = default;
	TextureComponent(const TextureComponent &) = default;
};

struct LightComponent
{
	// Color
	std::shared_ptr<Light> m_Light;

	// Type - Directional, Spot, Point
	LightComponent() = default;
	LightComponent(const LightComponent &) = default;
};

struct CameraComponent
{
	SceneCamera m_Camera;
	bool m_Primary = true;
	bool m_FixedAspectRatio = false;
	std::string skyboxPath = "";

	CameraComponent() = default;
	CameraComponent(const CameraComponent &) = default;
};

// PHYSICS
struct RigidbodyComponent
{
	// FLAG
	glm::vec3 m_FreezePos = {1, 1, 1};
	glm::vec3 m_FreezeRot = {1, 1, 1};

	void *body = nullptr;

	// body type
	enum class EBodyType
	{
		Static = 0,
		Dynamic,
		Kinematic
	};
	EBodyType m_Type = EBodyType::Dynamic;

	float m_Mass = 1.0f;
	float m_Damping = 0.001f;
	float m_AngularDamping = 0.001f;
	bool m_UseGravity = true;

	RigidbodyComponent() = default;
	RigidbodyComponent(const RigidbodyComponent &) = default;
};

struct BoxColliderComponent
{
	glm::vec3 m_Center = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_Size = {1.0f, 1.0f, 1.0f};
	bool m_IsTrigger = false;

	BoxColliderComponent() = default;
	BoxColliderComponent(const BoxColliderComponent &) = default;
};

struct SphereColliderComponent
{
	glm::vec3 m_Center;
	float m_Radius;
	bool m_IsTrigger = false;

	SphereColliderComponent() = default;
	SphereColliderComponent(const SphereColliderComponent &) = default;
};

struct CapsuleColliderComponent
{
	glm::vec3 m_Center;
	float m_Radius;
	float m_Height;

	bool m_IsTrigger = false;

	CapsuleColliderComponent() = default;
	CapsuleColliderComponent(const CapsuleColliderComponent &) = default;
};

struct CylinderColliderComponent
{
	glm::vec3 m_Center;
	float m_Radius;
	float m_Height;

	bool m_IsTrigger = false;

	CylinderColliderComponent() = default;
	CylinderColliderComponent(const CylinderColliderComponent &) = default;
};

// SCRIPTS
struct ScriptComponent
{
	std::string m_ClassName;

	ScriptComponent() = default;
	ScriptComponent(const ScriptComponent &) = default;
};

class ScriptableEntity;

struct NativeScriptComponent
{
	ScriptableEntity *instance = nullptr;

	ScriptableEntity *(*instantiateScript)();
	void (*destroyScript)(NativeScriptComponent *);

	template <typename T> void bind()
	{
		instantiateScript = []() {
			return static_cast<ScriptableEntity *>(new T());
		} destroyScript = [](NativeScriptComponent *nsc) {
			delete nsc->instance;
			nsc->instance = nullptr;
		}
	}
};

template <typename... Component> struct ComponentGroup
{
};

using AllComponents =
	ComponentGroup<TransformComponent, RelationshipComponent, MeshRendererComponent, TextureComponent, CameraComponent,
				   ScriptComponent, LightComponent, RigidbodyComponent, BoxColliderComponent, SphereColliderComponent,
				   CapsuleColliderComponent, CylinderColliderComponent, SkeletalAnimatorComponent>;

} // namespace ale

#endif