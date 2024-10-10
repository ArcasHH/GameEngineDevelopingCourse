#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsShooter.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void DeleteRenderObject(const EntitySystem::ECS::RenderThreadPtr* renderThread, RenderObjectPtr& renderObject)
{
	renderThread->ptr->EnqueueCommand(Render::ERC::DeleteRenderObject, GameEngine::RenderCore::Geometry::Ptr{}, renderObject.ptr);
}

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
		DeleteRenderObject(renderThread, renderObject);
		e.destruct();	
	});

	world.system< RenderObjectPtr, Collision, Bullet>()
		.each([&](flecs::entity e, RenderObjectPtr& renderObject, Collision& collision, Bullet& _)
	{
		if (!collision.is_collide )
			return;
		DeleteRenderObject(renderThread, renderObject);
		e.destruct();	
	});

	world.system< RenderObjectPtr, Health>()
		.each([&](flecs::entity e, RenderObjectPtr& renderObject, Health& health)
	{
		if (health.curr_health > 0.f)
			return;
		DeleteRenderObject(renderThread, renderObject);
		e.destruct();	
	});

}




