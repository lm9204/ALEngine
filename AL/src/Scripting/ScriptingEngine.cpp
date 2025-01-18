#include "Scripting/ScriptingEngine.h"

namespace ale
{
struct ScriptingEngineData
{
	// MonoDomain - 프로세스 내에 실행되는 어플리케이션들을 격리시키고, 각 작업을 독립적으로 실행하는 구조체.
	MonoDomain *rootDomain = nullptr;
	MonoDomain *appDomain = nullptr;
};

static ScriptingEngineData *s_Data = nullptr;

void ScriptingEngine::init()
{
	s_Data = new ScriptingEngineData();

	initMono();
}

void ScriptingEngine::shutDown()
{
}

void ScriptingEngine::initMono()
{
	// Validate check 필요.
	mono_set_assemblies_path("mono/lib");

	// Mono runtime 초기화
	MonoDomain *rootDomain = mono_jit_init("ALEJITRuntime"); // Manual delete 필요.

	// ASSERT로 대체.
	if (rootDomain == nullptr)
	{
		AL_CORE_ERROR("RootDomain doesn't exist!");
		return;
	}
	s_Data->rootDomain = rootDomain;

	// app domain create
	s_Data->appDomain = mono_domain_create_appdomain("ALEScriptRuntime", nullptr);
	mono_domain_set(s_Data->appDomain, true);
}

void ScriptingEngine::shutDownMono()
{
}
} // namespace ale