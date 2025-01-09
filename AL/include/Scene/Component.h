#ifndef COMPONENT_H
#define COMPONENT_H

#include "Scene/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ale
{
struct TagComponent
{
	std::string m_Tag;

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

	// 생성자
	TransformComponent() = default;
	TransformComponent(const TransformComponent &) = default;
	TransformComponent(const glm::vec3 &position) : m_Position(position)
	{
	}

	glm::mat4 getTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

		return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
	}
};

// RENDERER
struct ModelComponent
{
	// maybe mesh pointer or vector

	ModelComponent() = default;
	ModelComponent(const ModelComponent &) = default;
};

struct CamerComponent
{
	SceneCamera m_Camera;
	bool m_Primary = true;
	bool m_FixedAspectRatio = false;

	CamerComponent() = default;
	CamerComponent(const CamerComponent &) = default;
};

// PHYSICS
struct RigidbodyComponent
{
	RigidbodyComponent() = default;
	RigidbodyComponent(const RigidbodyComponent &) = default;
};

struct BoxColliderComponent
{
	BoxColliderComponent() = default;
	BoxColliderComponent(const BoxColliderComponent &) = default;
};

struct SphereColliderComponent
{
	SphereColliderComponent() = default;
	SphereColliderComponent(const SphereColliderComponent &) = default;
};

struct CapsuleColliderComponent
{
	CapsuleColliderComponent() = default;
	CapsuleColliderComponent(const CapsuleColliderComponent &) = default;
};

struct CylinderColliderComponent
{
	CylinderColliderComponent() = default;
	CylinderColliderComponent(const CylinderColliderComponent &) = default;
};

} // namespace ale

#endif