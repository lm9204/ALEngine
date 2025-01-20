#pragma once

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "Core/Timestep.h"

#include <filesystem>
#include <map>
#include <string>

// C 스타일의 name mangling을 사용하지 않도록 지시하는 키워드
// name mangling - 컴파일러가 변수나 함수의 고유 식별자를 생성하기 위해 사용되는 프로세스
// MyNamespace::MyClass::myFunction(int) → _ZN11MyNamespace7MyClass10myFunctionEi

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoString MonoString;
}

namespace ale
{
class ScriptClass
{
  public:
	ScriptClass() = default;
	ScriptClass(const std::string &classNamespace, const std::string &className, bool isCore = false);

	MonoObject *instantiate();
	MonoMethod *getMethod(const std::string &name, int parameterCount);
	MonoObject *invokeMethod(MonoObject *instance, MonoMethod *method, void **params = nullptr);

  private:
	std::string m_ClassNamespace;
	std::string m_ClassName;

	MonoClass *m_MonoClass = nullptr;
	friend class ScriptingEngine;
};

class ScriptInstance
{
  public:
	ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, Entity entity);

	void invokeOnCreate();
	void invokeOnUpdate(float ts);

	std::shared_ptr<ScriptClass> getScriptClass()
	{
		return m_ScriptClass;
	}

	MonoObject *getManagedObject()
	{
		return m_Instance;
	}

  private:
	std::shared_ptr<ScriptClass> m_ScriptClass;

	MonoObject *m_Instance = nullptr;
	MonoMethod *m_Constructor = nullptr;
	MonoMethod *m_OnCreateMethod = nullptr;
	MonoMethod *m_OnUpdateMethod = nullptr;

	friend class ScriptingEngine;
};

class ScriptingEngine
{
  public:
	static void init();
	static void shutDown();

	static bool loadAssembly(const std::filesystem::path &filepath);
	static bool loadAppAssembly(const std::filesystem::path &filepath);

	static void reloadAssembly();

	static void onRuntimeStart(Scene *scene);
	static void onRuntimeStop();

	static bool entityClassExists(const std::string &fullClassName);
	static void onCreateEntity(Entity entity);
	static void onUpdateEntity(Entity entity, Timestep ts);

	static Scene *getSceneContext();

	static MonoImage *getCoreAssemblyImage();

	static MonoObject *getManagedInstance(UUID uuid);

	static std::unordered_map<std::string, std::shared_ptr<ScriptClass>> getEntityClasses();

  private:
	static void initMono();
	static void shutDownMono();

	static MonoObject *instantiateClass(MonoClass *monoClass);
	static void loadAssemblyClasses();

	friend class ScriptClass;
	friend class ScriptGlue;
};
} // namespace ale