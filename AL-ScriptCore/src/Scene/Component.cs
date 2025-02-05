using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ALEngine
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_getPosition(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_setPosition(Entity.ID, ref value);
            }
        }
    }

    public class RigidbodyComponent : Component
    {
        public void addForce(Vector3 force)
        {
            InternalCalls.RigidbodyComponent_addForce(Entity.ID, ref force);
        }
    }
}
