#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>

namespace GameEngine
{
	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject() = default;

	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

		virtual void Update(size_t frame, float dt) = 0;

	protected:
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();
	};

	class PhysicObject : public GameObject
	{

	public:
		PhysicObject() = default;
		~PhysicObject() = default;
		PhysicObject(Math::Vector3f force, Math::Vector3f start_speed = Math::Vector3f::Zero())
		{
			m_Force = force;
			m_Speed = start_speed;
		}
		void Update(size_t frame, float dt)
		{
			Math::Vector3f curr_pos = GetPosition();
			if (curr_pos.y > 0.f)
			{
				m_Speed = m_Speed + m_Force;
			}
			else
			{
				m_Speed.y = - m_Speed.y;
			}
			SetPosition(curr_pos + m_Speed, frame);
		}
	protected:
		Math::Vector3f m_Speed = Math::Vector3f::Zero();
		Math::Vector3f m_Force = Math::Vector3f::Zero();
	};

	class MovingObject : public GameObject
	{

	public:
		MovingObject() = default;
		~MovingObject() = default;
		MovingObject(Math::Vector3f center, Math::Vector3f start_speed = Math::Vector3f::Zero())
		{
			m_Center = center;
			m_Speed = start_speed;
		}
		void Update(size_t frame, float dt)
		{
			Math::Vector3f dpos = -GetPosition() + m_Center;
			//the direction depends on the starting position
			m_Speed = m_Speed + dpos*0.001f;
			SetPosition(GetPosition() + m_Speed, frame);
		}
	protected:
		Math::Vector3f m_Speed = Math::Vector3f::Zero();
		Math::Vector3f m_Center = Math::Vector3f::Zero(); // center, relative to which it oscillates back and forth. 
	};

	class PlayableObject : public GameObject
	{

	public:
		PlayableObject() = default;
		~PlayableObject() = default;

		void Update(size_t frame, float dt)
		{
			
		}
	protected:
		Math::Vector3f m_Speed = Math::Vector3f::Zero();
	};
}