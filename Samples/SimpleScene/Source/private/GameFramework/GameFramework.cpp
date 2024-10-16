// This define is essential for the scripts to expose the ECS components to lua syntax
// There is a task to rework this behavior
#define GAME_FRAMEWORK

#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsShooter.h>
#include <ECS/ecsSystems.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>
#include <flecs.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterComponents();
	RegisterSystems();

	flecs::entity cubeControl = m_World.entity()
		.set(Position{ -2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })

		.set(CollisionSize{1.f,1.f,1.f})
		.set(IsCollide{false})
		.set(ApplyForce{0.0f, 0.0f, 0.0f})
		;

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ 2.f, 0.f, 0.f })
		.set(Velocity{ 0.f, 5.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })

		.set(CollisionSize{1.f,1.f,1.f})
		.set(IsCollide{false})
		.set(ApplyForce{0.0f, 0.0f, 0.0f})
		;

	flecs::entity cubeMoving2 = m_World.entity()
		.set(Position{ 1.f, 10.f, 1.f })
		.set(Velocity{ 0.f, 3.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 0.3f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })

		.set(Timer{ 0.f, 5.f })
		.set(Visibility{true})
		//if another components - collisions doesnt work ?????
		.set(CollisionSize{1.f,1.f,1.f})
		.set(IsCollide{false})
		.set(ApplyForce{0.0f, 0.0f, 0.0f})
		;

	flecs::entity cubeMoving3 = m_World.entity()
		.set(Position{ 2.f, 15.f, 0.f })
		.set(Velocity{ 0.f, 0.f, 0.f })
		.set(Gravity{ 0.f, -9.8065f, 0.f })
		.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
		.set(Bounciness{ 1.f })
		.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
		.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })

		.set(CollisionSize{1.f,1.f,1.f})
		.set(IsCollide{false})
		.set(ApplyForce{0.0f, 0.0f, 0.0f})
		;

	flecs::entity camera = m_World.entity()
		.set(Position{ 0.0f, 12.0f, -10.0f })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })

		.set(Shot{ 0.f, 1.f, false })
		;

	m_World.system<const Position, Shot>()
		.each([&](const Position pos, Shot& gun)
	{
		if (!gun.value)
			return;
		Math::Vector3f vel = Core::g_MainCamera->GetViewDir().Normalized() * 50.f;
		m_World.entity()
			.set(Position{ pos.x, pos.y, pos.z  })
			.set(Velocity{ vel.x, vel.y, vel.z })
			.set(FrictionAmount{ .8f })
			.set(Gravity{ 0.f, -9.8065f, 0.f })
			.set(BouncePlane{ 0.f, 1.f, 0.f, 0.1f })
			.set(Bounciness{ 1.f })
			.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube(0.1f, 0.1f, 0.1f) })
			.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() })

			.set(CollisionSize{0.1f, 0.1f, 0.1f})
			.set(IsCollide{false})
			.set(ApplyForce{0.0f, 0.0f, 0.0f})

			.set(Timer{ 0.f, 5.f })
			.set(Visibility{true})

			.set(Bullet{1.f})
		;
		gun.value = false;
	});
}

void GameFramework::RegisterComponents()
{
	// Exposing these components for the lua system
	ECS_META_COMPONENT(m_World, Position);
	ECS_META_COMPONENT(m_World, Velocity);
	ECS_META_COMPONENT(m_World, Gravity);
	ECS_META_COMPONENT(m_World, BouncePlane);
	ECS_META_COMPONENT(m_World, Bounciness);
	ECS_META_COMPONENT(m_World, ShiverAmount);
	ECS_META_COMPONENT(m_World, FrictionAmount);
	ECS_META_COMPONENT(m_World, Speed);

	ECS_META_COMPONENT(m_World, Timer);
	ECS_META_COMPONENT(m_World, Visibility);

	ECS_META_COMPONENT(m_World, CollisionSize);
	ECS_META_COMPONENT(m_World, IsCollide);
	ECS_META_COMPONENT(m_World, ApplyForce);

	ECS_META_COMPONENT(m_World, Bullet);
}

void GameFramework::RegisterSystems()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsShooterSystems(m_World);
}

void GameFramework::Update(float dt)
{
}