#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsMeshSystems(flecs::world& world)
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<EntitySystem::ECS::RenderObjectPtr, const Position>()
		.each([&](EntitySystem::ECS::RenderObjectPtr& renderObject, const Position& position)
	{
		if (renderObject.ptr) [[likely]]
		{
			renderObject.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z), renderThread->ptr->GetMainFrame());
		}
	});

	world.system<EntitySystem::ECS::RenderObjectPtr, const Rotation>()
		.each([&](EntitySystem::ECS::RenderObjectPtr& renderObject, const Rotation& rotation)
	{
		if (renderObject.ptr) [[likely]]
		{
			renderObject.ptr->SetRotation(Math::Vector3f(rotation.x, rotation.y, rotation.z), rotation.angle, renderThread->ptr->GetMainFrame());
		}
	});
}
