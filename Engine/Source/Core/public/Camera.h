#pragma once

#include <Core/export.h>
#include <Matrix.h>
#include <Vector.h>

namespace GameEngine
{
	namespace Core
	{
		class CORE_API Camera final
		{
		public:
			Camera() = default;

		public:
			Math::Matrix4x4f GetViewMatrix();
			Math::Vector3f GetPosition() const { return m_Position; }
			void SetPosition(Math::Vector3f position) { m_Position = position; }
			Math::Vector3f GetViewDir() const { return m_ViewDir; }
			void SetViewDir(Math::Vector3f viewDir) { m_ViewDir = viewDir; }
			void Rotate(float yaw, float pitch);
			void SetSpeed(Math::Vector3f newSpeed) { m_Speed = newSpeed; }
			Math::Vector3f GetSpeed() { return m_Speed; }

			void Update(float dt);

		private:
			Math::Vector3f m_Position;
			Math::Vector3f m_ViewDir;
			Math::Vector3f m_Speed;
		};

		extern CORE_API Camera* g_MainCamera;
	}
}