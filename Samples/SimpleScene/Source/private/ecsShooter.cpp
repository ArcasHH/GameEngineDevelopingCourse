#include <ecsShooter.h>
#include <ecsPhys.h>
#include <ecsMesh.h>
#include <flecs.h>

void RegisterEcsShooterSystems(flecs::world& world)
{
   
	world.system<const Position, const BouncePlane, DespawnOnBouncePlane, RenderObjectPtr>()
        .each([&](flecs::entity e, const Position& pos, const BouncePlane& plane, DespawnOnBouncePlane& d, const RenderObjectPtr& renderObject) 
    {
        constexpr float planeEpsilon = 0.1f;
		if (plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z  < plane.value.w + planeEpsilon)
        {
            d.timer = 0.f;
        }
        else 
        {
            d.timer += world.delta_time();
        }
        if (d.timer > d.despawn_time) 
        {
            // Destroy here
        }
    });
}