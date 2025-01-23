#include "alpch.h"

#include "Scripting/ScriptingEngine.h"
#include "Scripting/ScriptingGlue.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/UUID.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

namespace ale
{
namespace utils
{

std::string monoStringToString(MonoString *string)
{
	char *cStr = mono_string_to_utf8(string);
	std::string str(cStr);
	mono_free(cStr);
	return str;
}

} // namespace utils

static std::unordered_map<MonoType *, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("ALEngine.InternalCalls::" #Name, Name)

// Native Function API

// Log
static void nativeLog(MonoString *string, int parameter)
{
	std::string str = utils::monoStringToString(string);
	std::cout << str << '\n';
}

static void nativeLog_Vector(glm::vec3 *parameter, glm::vec3 *outResult)
{
	// AL_CORE_WARN("Value: {0}", *parameter);
	*outResult = glm::normalize(*parameter);
}

// Entity
static bool Entity_hasComponent(UUID entityID, MonoReflectionType *componentType)
{
	// ASSERT
	Scene *scene = ScriptingEngine::getSceneContext();
	Entity entity = scene->createEntityWithUUID(entityID);

	MonoType *managedType = mono_reflection_type_get_type(componentType);
	return s_EntityHasComponentFuncs.at(managedType)(entity);
}

static uint64_t Entity_findEntityByName(MonoString *name)
{
	char *nameCStr = mono_string_to_utf8(name);

	Scene *scene = ScriptingEngine::getSceneContext();
	Entity entity = scene->findEntityByName(nameCStr);
	mono_free(nameCStr);

	if (!entity)
	{
		return 0;
	}
	return entity.getUUID();
}

static MonoObject *getScriptInstance(UUID entityID)
{
	return ScriptingEngine::getManagedInstance(entityID);
}

// Transform
static void TransformComponent_getPosition(UUID entityID, glm::vec3 *outPosition)
{
	Scene *scene = ScriptingEngine::getSceneContext();
	Entity entity = scene->getEntityByUUID(entityID);

	*outPosition = entity.getComponent<TransformComponent>().m_Position;
}

static void TransformComponent_setPosition(UUID entityID, glm::vec3 *position)
{
	Scene *scene = ScriptingEngine::getSceneContext();
	Entity entity = scene->getEntityByUUID(entityID);

	entity.getComponent<TransformComponent>().m_Position = *position;
}

// Input
static bool Input_isKeyDown(KeyCode keycode)
{
	return Input::isKeyPressed(keycode);
}

// Component 별로 HasComponentFunction handle 등록.
template <typename... Component> static void registerComponent()
{
	(
		[]() {
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("ALEngine.{}", structName);

			MonoType *managedType =
				mono_reflection_type_from_name(managedTypename.data(), ScriptingEngine::getCoreAssemblyImage());
			if (!managedType)
			{
				// AL_CORE_ERROR("Could not find component type");
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.hasComponent<Component>(); };
		}(),
		...);
}

template <typename... Component> static void registerComponent(ComponentGroup<Component...>)
{
	registerComponent<Component...>();
}

void ScriptGlue::registerComponents()
{
	s_EntityHasComponentFuncs.clear();
	registerComponent(AllComponents{});
}

void ScriptGlue::registerFunctions()
{
	ADD_INTERNAL_CALL(nativeLog);
	ADD_INTERNAL_CALL(nativeLog_Vector);

	ADD_INTERNAL_CALL(Entity_hasComponent);
	ADD_INTERNAL_CALL(Entity_findEntityByName);
	ADD_INTERNAL_CALL(getScriptInstance);

	ADD_INTERNAL_CALL(TransformComponent_getPosition);
	ADD_INTERNAL_CALL(TransformComponent_setPosition);

	ADD_INTERNAL_CALL(Input_isKeyDown);
}
} // namespace ale