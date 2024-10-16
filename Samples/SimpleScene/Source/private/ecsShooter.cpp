#include <ecsShooter.h>

#include <Geometry.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <RenderThread.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsShooterSystems(flecs::world& world) 
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<EntitySystem::ECS::RenderObjectPtr, Visibility>()
		.each([&](flecs::entity e, EntitySystem::ECS::RenderObjectPtr& renderObject, Visibility visibility)
	{
			if (!visibility.is_visible)
			{
				renderThread->ptr->EnqueueCommand(Render::ERC::DeleteRenderObject, GameEngine::RenderCore::Geometry::Ptr{}, renderObject.ptr);	
				e.destruct();
			}
	});
}