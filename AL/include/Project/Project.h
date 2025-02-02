#pragma once

#include <filesystem>
#include <string>

namespace ale
{
struct ProjectConfig
{
	std::string m_Name = "Untitled";

	std::filesystem::path m_ProjectPath;
	std::filesystem::path m_StartScene;

	std::filesystem::path m_AssetDirectory;
	std::filesystem::path m_ScriptModulePath;
	std::filesystem::path m_ScriptCorePath;
};

class Project
{
  public:
	static const std::filesystem::path &getProjectDirectory()
	{
		// ASSERT
		return s_ActiveProject->m_ProjectDirectory;
	}

	static std::filesystem::path getAssetDirectory()
	{
		// ASSERT
		return getProjectDirectory() / s_ActiveProject->m_Config.m_AssetDirectory;
	}

	static std::filesystem::path getAssetFileSystemPath(const std::filesystem::path &path)
	{
		// ASSERT
		return getAssetDirectory() / path;
	}

	ProjectConfig &getConfig()
	{
		return m_Config;
	}

	static std::shared_ptr<Project> getActive()
	{
		return s_ActiveProject;
	}

	static std::shared_ptr<Project> create();
	static std::shared_ptr<Project> load(const std::filesystem::path &path);
	static bool saveActive(const std::filesystem::path &path);

  private:
	ProjectConfig m_Config;
	std::filesystem::path m_ProjectDirectory;

	inline static std::shared_ptr<Project> s_ActiveProject;
};
} // namespace ale