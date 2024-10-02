#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>

namespace GameEngine
{
	class GameObject;

	struct ControllerInterface 
	{
		virtual void Update(GameObject*, size_t, float) = 0;
		virtual void OnAttachment(GameObject*) = 0;
		virtual ~ControllerInterface() = default;
	};

	class GameObject
	{
	public:
		GameObject(std::shared_ptr<ControllerInterface> ctr) : ctr{ ctr } 
		{ 
			ctr->OnAttachment(this); 
		};
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

		void SetSpeed(Math::Vector3f speed)
		{
			m_Speed = speed;
		}
		Math::Vector3f GetSpeed()
		{
			return m_Speed;
		}

		virtual void Update(size_t frame, float dt)
		{
			ctr->Update(this, frame, dt);
			SetPosition(GetPosition() + GetSpeed() * dt, frame);
		}
	protected:	
		std::shared_ptr<ControllerInterface> ctr;
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Speed = Math::Vector3f::Zero();
		Math::Vector3f m_Position = Math::Vector3f::Zero();
	};

	class PhysicController : public ControllerInterface 
	{
		void Update(GameObject* obj, size_t frame, float dt) 
		{
			float plane_y = -56.f;
			if (obj->GetPosition().y > plane_y) // imaginary plane
			{
				obj->SetSpeed( obj->GetSpeed() + Math::Vector3f{ 0.f, -50.f, 0.f } * dt); 
			}
			else
			{
				obj->SetSpeed ( Math::Vector3f(obj->GetSpeed().x, -obj->GetSpeed().y, obj->GetSpeed().z) ); // by plane y = const
				obj->SetPosition ( Math::Vector3f(obj->GetPosition().x, plane_y, obj->GetPosition().z), frame);// for problems if starting position << plane_y
			}
		}
		void OnAttachment(GameObject* obj) override { }
	};

	class MovingController : public ControllerInterface 
	{
		void Update(GameObject* obj, size_t frame, float dt) 
		{
			Math::Vector3f dir =  Math::Vector3f{ 0.f, 0.f, 0.f - obj->GetPosition().z }; // for moving back-forward by z 
			obj->SetSpeed( obj->GetSpeed() + dir * 0.01f); 
		}
		void OnAttachment(GameObject* obj) override { }
	};

	class KeyboardContoller : public ControllerInterface 
	{
		std::vector<GameObject*> objects;
		void Update(GameObject* obj, size_t frame, float dt) 
		{
			obj->SetSpeed(obj->GetSpeed() * 0.95); //speed fading
		}
		void OnAttachment(GameObject* obj) override 
		{ 
			objects.push_back(obj); 
		}
	public:
		template <typename CallbackFunc>
		void UpdateAll(CallbackFunc &&func) 
		{
			for (GameObject* obj : objects)
				std::invoke(func, obj);
		}
	};
}