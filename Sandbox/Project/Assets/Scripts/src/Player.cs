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
        // private TransformComponent m_Transform;

        public float Speed;
        public float Time = 0.0f;

        void onCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            // m_Transform = getComponent<TransformComponent>();
        }

        void onUpdate(float ts)
        {
            Time += ts;
            Console.WriteLine($"Player.OnUpdate: {ts}");
        }

    }
}
