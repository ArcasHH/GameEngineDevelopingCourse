#pragma once
#include <Camera.h>
#include <DefaultGeometry.h>

#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsShooter.h>

#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>

using namespace GameEngine;
constexpr Math::Vector3f size1 = Math::Vector3f(2.f, 1.f, 1.f);
constexpr Math::Vector3f size2 = Math::Vector3f(2.f, 5.f, 1.f);

void GameFramework::Init()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsPhysSystems(m_World);
	RegisterEcsShooterSystems(m_World);

	flecs::entity cubeControl = m_World.entity()
		.set(Position{ Math::Vector3f(-2.f, 0.f, 0.f) })
		.set(Velocity{ Math::Vector3f(0.f, 0.f, 0.f) })
		.set(Speed{ 10.f })
		.set(FrictionAmount{ 0.9f })
		.set(JumpSpeed{ 10.f })
		.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
		.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
		.set(Bounciness{ 0.3f })
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube(size1) })
		.set(RenderObjectPtr{ new Render::RenderObject() })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	flecs::entity cubeMoving = m_World.entity()
		.set(Position{ Math::Vector3f(2.f, 0.f, 0.f) })
		.set(Velocity{ Math::Vector3f(0.f, 2.f, 0.f) })
		.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
		.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
		.set(Bounciness{ 1.f })
		.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube(size2) })
		.set(RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity camera = m_World.entity()
		.set(Position{ Math::Vector3f(0.0f, 12.0f, -10.0f) })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) })
		.set(Gun{ 0.f, 0.5f,true, false})
		;
}

void GameFramework::Update(float dt)
{
	m_World.system<const Position, Gun>()
		.each([&](const Position position, Gun& gun)
	{
		if (!gun.is_shoot)
			return;
		m_World.entity()
			.set(Position{ position.value })
			.set(Velocity{ Core::g_MainCamera->GetViewDir().Normalized() * 20.f })
			.set(FrictionAmount{ 0.9f })
			.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
			.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
			.set(Bounciness{ 1.f })
			.set(DespawnOnBouncePlane{0.f, 5.f})
			.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube(Math::Vector3f(0.1f, 0.1f, 0.1f)) })
			.set(RenderObjectPtr{ new Render::RenderObject() 
		});
		gun.is_shoot = false;
	});
			
}