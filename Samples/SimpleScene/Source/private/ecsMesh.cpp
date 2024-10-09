#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsShooter.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsMeshSystems(flecs::world& world)
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<const GeometryPtr, RenderObjectPtr>()
		.each([&](flecs::entity e, const GeometryPtr& geometry, RenderObjectPtr& renderObject)
	{
		renderThread->ptr->EnqueueCommand(Render::ERC::CreateRenderObject, geometry.ptr, renderObject.ptr);
		e.remove<GeometryPtr>();
	});

	world.system<RenderObjectPtr, const Position>()
		.each([&](RenderObjectPtr& renderObject, const Position& position)
	{
		renderObject.ptr->SetPosition(position.value, renderThread->ptr->GetMainFrame());
	});
	
	world.system< RenderObjectPtr, DespawnOnBouncePlane>()
		.each([&](flecs::entity e, RenderObjectPtr& renderObject, DespawnOnBouncePlane& despawn)
	{
		if (despawn.timer < despawn.despawn_time)
			return;
		// despawn bullet
		GameEngine::RenderCore::Geometry::Ptr fict_geom_ptr;
		renderThread->ptr->EnqueueCommand(Render::ERC::DeleteRenderObject, fict_geom_ptr, renderObject.ptr);
		e.destruct();	
	});
}


