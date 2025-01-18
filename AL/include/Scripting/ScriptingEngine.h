#pragma once

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace ale
{
class ScriptingEngine
{
  public:
	static void init();
	static void shutDown();

  private:
	static void initMono();
	static void shutDownMono();
};
} // namespace ale