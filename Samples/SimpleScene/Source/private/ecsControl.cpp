#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <Quaternion.h>

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
	{
		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
		}
		position.x = position.x + currentMoveDir.Normalized().x * speed.value * world.delta_time();
		position.y = position.y + currentMoveDir.Normalized().y * speed.value * world.delta_time();
		position.z = position.z + currentMoveDir.Normalized().z * speed.value * world.delta_time();
		camera.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z));
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.y = jump.value;
			}
		}
	});

	//an example of using quaternions to move around (0,0,0)
	world.system<Position, const ControllerPtr, onRotate>()
		.each([&]( Position& pos, const ControllerPtr& controller, onRotate& rot)
	{
		static float timer;
		if (controller.ptr->IsPressed("Rotate"))
		{
			rot.isRotate = true;
			timer = 0.f;
		}
		if (rot.timer < timer && rot.isRotate)
		{
			rot.isRotate = false;
			timer = 0.f;
		}
		if (rot.isRotate)
		{
			Math::Vector3f newPos(pos.x, pos.y, pos.z);
			Math::RotateVec3(newPos, Math::Vector3f(0.f, 1.f, -1.f), rot.speed);
			pos.x = newPos.x;
			pos.y = newPos.y;
			pos.z = newPos.z;
			timer += world.delta_time();
		}
	});
}

