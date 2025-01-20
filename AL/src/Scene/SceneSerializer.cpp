#include "alpch.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/SceneSerializer.h"

#include <yaml-cpp/yaml.h>

namespace YAML
{
template <> struct convert<glm::vec2>
{
	static Node encode(const glm::vec2 &rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node &node, glm::vec2 &rhs)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}
};

template <> struct convert<glm::vec3>
{
	static Node encode(const glm::vec3 &rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node &node, glm::vec3 &rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}
};

template <> struct convert<glm::vec4>
{
	static Node encode(const glm::vec4 &rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node &node, glm::vec4 &rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}
};
} // namespace YAML

namespace ale
{

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec2 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec3 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

YAML::Emitter &operator<<(YAML::Emitter &out, const glm::vec4 &v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
	return out;
}

SceneSerializer::SceneSerializer(const std::shared_ptr<Scene> &scene) : m_Scene(scene)
{
}

static void serializeEntity(YAML::Emitter &out, Entity entity)
{
	out << YAML::BeginMap;
	out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

	// KEY, KEY - VALUE
	if (entity.hasComponent<TagComponent>())
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap;
		auto &tag = entity.getComponent<TagComponent>().m_Tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;
		out << YAML::EndMap;
	}

	// TransformComponent
	if (entity.hasComponent<TransformComponent>())
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap;
		auto &tf = entity.getComponent<TransformComponent>();
		out << YAML::Key << "Position" << YAML::Value << tf.m_Position;
		out << YAML::Key << "Rotation" << YAML::Value << tf.m_Rotation;
		out << YAML::Key << "Scale" << YAML::Value << tf.m_Scale;
		out << YAML::EndMap;
	}
	// CameraComponent
	if (entity.hasComponent<CameraComponent>())
	{
		out << YAML::Key << "CameraComponent";
		out << YAML::BeginMap;

		auto &cc = entity.getComponent<CameraComponent>();
		auto &camera = cc.m_Camera;
		out << YAML::Key << "Camera";
		out << YAML::BeginMap;
		out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
		out << YAML::EndMap;

		out << YAML::Key << "Primary" << YAML::Value << cc.m_Primary;
		out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.m_FixedAspectRatio;

		out << YAML::EndMap;
	}

	// MeshRendererComponent
	// LightComponent
	// RigidbodyComponent
	// BoxColliderComponent
	// SphereColliderComponent
	// CapsuleColliderComponent
	// CylinderColliderComponent
	// ScriptComponent
	if (entity.hasComponent<ScriptComponent>())
	{
		auto &scriptComponent = entity.getComponent<ScriptComponent>();

		out << YAML::Key << "ScriptComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.m_ClassName;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;
}

void SceneSerializer::serialize(const std::string &filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID) {
		Entity entity{entityID, m_Scene.get()};
		if (!entity)
		{
			return;
		}
		serializeEntity(out, entity);
	});

	out << YAML::EndSeq;
	out << YAML::EndMap;
	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::serializeRuntime(const std::string &filepath)
{
}

bool SceneSerializer::deserialize(const std::string &filepath)
{
	YAML::Node data;

	try
	{
		data = YAML::LoadFile(filepath);
	}
	catch (const std::exception &e)
	{
		AL_CORE_ERROR("Failed to load .ale file '{0}'\n    {1}", filepath, e.what());
		return false;
	}

	// Scene
	if (!data["Scene"])
	{
		return false;
	}

	// Entities
	auto entities = data["Entities"];
	if (entities)
	{
		for (auto entity : entities)
		{
			// IDComponent
			uint64_t uuid = entity["Entity"].as<uint64_t>();

			// TagComponent
			std::string name;
			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
			{
				name = tagComponent["Tag"].as<std::string>();
			}

			AL_CORE_TRACE("Entity deserialized as ID: {0}, Tag: {1}", uuid, name);

			Entity deserializedEntity = m_Scene->createEntityWithUUID(uuid, name);

			// TransformComponent
			auto tfComponent = entity["TransformComponent"];
			if (tfComponent)
			{
				auto &tf = deserializedEntity.getComponent<TransformComponent>();
				tf.m_Position = tfComponent["Position"].as<glm::vec3>();
				tf.m_Rotation = tfComponent["Rotation"].as<glm::vec3>();
				tf.m_Scale = tfComponent["Scale"].as<glm::vec3>();
			}

			// CameraComponent
			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto &cc = deserializedEntity.addComponent<CameraComponent>();
				auto &cameraProps = cameraComponent["Camera"];

				cc.m_Camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
				cc.m_Camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
				cc.m_Camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

				cc.m_Primary = cameraProps["Primary"].as<bool>();
				cc.m_FixedAspectRatio = cameraProps["FixedAspectRatio"].as<bool>();
			}

			// MeshRendererComponent
			// LightComponent
			// RigidbodyComponent
			// BoxColliderComponent
			// SphereColliderComponent
			// CapsuleColliderComponent
			// CylinderColliderComponent
			// ScriptComponent
			auto scriptComponent = entity["ScriptComponent"];
			if (scriptComponent)
			{
				auto &sc = deserializedEntity.addComponent<ScriptComponent>();
				sc.m_ClassName = scriptComponent["ClassName"].as<std::string>();
			}
		}
	}
	return true;
}

bool SceneSerializer::deserializeRuntime(const std::string &filepath)
{
	return true;
}

} // namespace ale