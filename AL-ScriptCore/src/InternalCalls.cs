using System;
using System.Runtime.CompilerServices;

namespace ALEngine
{
	public static class InternalCalls
	{
		// internal
		// 멤버가 같은 어셈블리에서만 접근 가능.

		// MethodImplAttribute
		// .NET 런타임에 메서드 구현 세부 정보를 제공하는 특성.

		// MethodImplOptions.InternalCall
		// 해당 메서드가 런타임에 네이티브 구현임을 나타냄.

		#region Entity
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_hasComponent(ulong entityID, Type componentType);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static ulong Entity_findEntityByName(string name);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static object getScriptInstance(ulong entityID);
		#endregion

		#region TransformComponent
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_getPosition(ulong entityID, out Vector3 translation);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_setPosition(ulong entityID, ref Vector3 translation);
		#endregion

		#region Input
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidbodyComponent_addForce(ulong entityID, ref Vector3 force);
		#endregion

		#region Input
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_isKeyDown(KeyCode keycode);
		#endregion
	}
}