using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ALEngine;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_Transform;
        private RigidbodyComponent m_Rigidbody;

        public float Speed;
        public float Time = 0.0f;

        void onCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            m_Transform = getComponent<TransformComponent>();
            m_Rigidbody = getComponent<RigidbodyComponent>();
        }

        void onUpdate(float ts)
        {
            Vector3 velocity = Vector3.Zero;

            if (Input.isKeyDown(KeyCode.W))
            {
                velocity.Z = -1.0f;
            }
            else if (Input.isKeyDown(KeyCode.S))
            {
                velocity.Z = 1.0f;
            }

            if (Input.isKeyDown(KeyCode.A))
            {
                velocity.X = -1.0f;
            }
            else if (Input.isKeyDown(KeyCode.D))
            {
                velocity.X = 1.0f;
            }

            velocity *= Speed * ts;
            m_Rigidbody.addForce(velocity.XYZ);
        }

    }
}
