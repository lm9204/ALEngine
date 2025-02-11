using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ALEngine;

namespace Sandbox
{
    public class Camera : Entity
    {
        private TransformComponent m_Transform;

        public float RotSpeed;
        public float Speed;

        void onCreate()
        {
            Console.WriteLine($"Camera.OnCreate - {ID}");

            m_Transform = getComponent<TransformComponent>();
        }

        void onUpdate(float ts)
        {
            Vector3 velocity = Vector3.Zero;
            Vector3 rotVelocity = Vector3.Zero;

            if (Input.isKeyDown(KeyCode.Q))
            {
                rotVelocity.X = -1.0f;
            }
            else if (Input.isKeyDown(KeyCode.E))
            {
                rotVelocity.X = 1.0f;
            }

            if (Input.isKeyDown(KeyCode.Up))
            {
                velocity.Z = -1.0f;
            }
            else if (Input.isKeyDown(KeyCode.Down))
            {
                velocity.Z = 1.0f;
            }

            if (Input.isKeyDown(KeyCode.Left))
            {
                velocity.X = -1.0f;
            }
            else if (Input.isKeyDown(KeyCode.Right))
            {
                velocity.X = 1.0f;
            }

            Vector3 rotation = m_Transform.Rotation;
            rotation += rotVelocity * RotSpeed * ts;
            m_Transform.Rotation = rotation;

            Vector3 translation = m_Transform.Translation;
            translation += velocity * Speed * ts;
            m_Transform.Translation = translation;
        }

    }
}
