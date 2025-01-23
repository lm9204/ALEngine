#include "alpch.h"

#include "Project/Project.h"
#include "Project/ProjectSerializer.h"

namespace ale
{
std::shared_ptr<Project> Project::create()
{
	s_ActiveProject = std::make_shared<Project>();
	return s_ActiveProject;
}

std::shared_ptr<Project> Project::load(const std::filesystem::path &path)
{
	if (path.extension().string() != ".alproj")
	{
		AL_CORE_WARN("Could not load {0} - not a project file", path.filename().string());
		return nullptr;
	}

	std::shared_ptr<Project> project = std::make_shared<Project>();

	ProjectSerializer serializer(project);
	if (serializer.deserialize(path))
	{
		project->m_ProjectDirectory = path.parent_path();
		s_ActiveProject = project;
		return s_ActiveProject;
	}

	return nullptr;
}

bool Project::saveActive(const std::filesystem::path &path)
{
	ProjectSerializer serializer(s_ActiveProject);
	if (serializer.serialize(path))
	{
		s_ActiveProject->m_ProjectDirectory = path.parent_path();
		return true;
	}

	return false;
}
} // namespace ale