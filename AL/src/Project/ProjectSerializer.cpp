#include "alpch.h"

#include "Project/ProjectSerializer.h"
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace ale
{
ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project) : m_Project(project)
{
}

bool ProjectSerializer::serialize(const std::filesystem::path &filepath)
{
	const auto &config = m_Project->getConfig();

	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap; // Project
			out << YAML::Key << "Name" << YAML::Value << config.m_Name;
			out << YAML::Key << "StartScene" << YAML::Value << config.m_StartScene.string();
			out << YAML::Key << "AssetDirectory" << YAML::Value << config.m_AssetDirectory.string();
			out << YAML::Key << "ScriptModulePath" << YAML::Value << config.m_ScriptModulePath.string();
			out << YAML::EndMap; // Project
		}
		out << YAML::EndMap; // Root
	}

	std::ofstream fout(filepath);
	fout << out.c_str();

	return true;
}

bool ProjectSerializer::deserialize(const std::filesystem::path &filepath)
{
	auto &config = m_Project->getConfig();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(filepath.string());
	}
	catch (YAML::ParserException e)
	{
		AL_CORE_ERROR("Failed to load project file\n     {0}", e.what());
		return false;
	}

	auto projectNode = data["Project"];
	if (!projectNode)
		return false;

	config.m_Name = projectNode["Name"].as<std::string>();
	// ProjectPath를 상대 경로로 변환
	std::filesystem::path projectPath = projectNode["ProjectPath"].as<std::string>();
	if (projectPath.is_absolute())
	{
		config.m_ProjectPath = projectPath;
	}
	else
	{
		config.m_ProjectPath = std::filesystem::relative(projectPath, "projects");
	}
	config.m_StartScene = projectNode["StartScene"].as<std::string>();
	config.m_AssetDirectory = config.m_ProjectPath / projectNode["AssetDirectory"].as<std::string>();
	config.m_ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
	config.m_ScriptCorePath = projectNode["ScriptCorePath"].as<std::string>();
	return true;
}
} // namespace ale