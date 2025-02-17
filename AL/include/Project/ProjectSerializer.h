#pragma once

#include "Project/Project.h"

namespace ale
{
class ProjectSerializer
{
  public:
	ProjectSerializer(std::shared_ptr<Project> project);

	bool serialize(const std::filesystem::path &filepath);
	bool deserialize(const std::filesystem::path &filepath);

  private:
	std::shared_ptr<Project> m_Project;
};
} // namespace ale