#pragma once

#include "Core/Buffer.h"

#include <filesystem>

namespace ale
{
class FileSystem
{
  public:
	static SBuffer readFileBinary(const std::filesystem::path &filepath);
};
} // namespace ale