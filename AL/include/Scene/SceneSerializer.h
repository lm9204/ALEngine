#pragma once

#include "Scene/Scene.h"
#include <unordered_map>

namespace ale
{
struct RelationshipData
{
	uint64_t entityUUID;
	uint64_t parentUUID;
	std::vector<uint64_t> childrenUUIDs;
};

class SceneSerializer
{
  public:
	SceneSerializer(const std::shared_ptr<Scene> &scene);

	void serialize(const std::string &filepath);
	void serializeRuntime(const std::string &filepath);

	bool deserialize(const std::string &filepath);
	bool deserializeRuntime(const std::string &filepath);

  private:
	std::shared_ptr<Scene> m_Scene;
	std::unordered_map<uint64_t, RelationshipData> relationshipMap;
};

} // namespace ale