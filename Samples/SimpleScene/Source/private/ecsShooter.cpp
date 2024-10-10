#include <ecsShooter.h>
#include <ecsPhys.h>

#include <flecs.h>

void RegisterEcsShooterSystems(flecs::world& world)
{

	world.system< Gun>()
		.each([&](flecs::entity e, Gun& gun)
	{
		if (gun.ammo == 0 && gun.reloaded)
		{
			gun.reloaded = false;
		}
		if (!gun.reloaded)
		{
			gun.timer += world.delta_time();
			if (gun.timer > gun.reload_time)
			{
				gun.reloaded = true;
				gun.ammo = gun.max_ammo;
				gun.timer = 0.f;
			}
		}
	});

	world.system< Gun, FireRate>()
		.each([&](flecs::entity e, Gun& gun, FireRate& fire)
	{

		if (gun.reloaded && fire.timer > fire.time_for_one_shot)
		{
			fire.can_shoot = true;
			fire.timer = 0.f;
		}
		else
		{
			fire.can_shoot = false;
			fire.timer += world.delta_time();
		}
	});


	world.system<Position, Velocity, BouncePlane, DespawnOnBouncePlane>()
        .each([&](flecs::entity e, const Position& pos, Velocity& vel, const BouncePlane& plane, DespawnOnBouncePlane& despawn) 
    {
        constexpr float planeEpsilon = 0.2f;
		float dotPos = plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z;
		if ((dotPos  < plane.value.w + planeEpsilon) && 
			!despawn.collide_ground)
        {
			despawn.collide_ground = true;
			vel.value = vel.value * 0.02;
			despawn.timer = 0.f;
        }
		if (despawn.collide_ground)
		{
			despawn.timer += world.delta_time();
		}
    });

	world.system<Position, CollisionSize, Collision>()
		.each([&](flecs::entity e1, Position& bullet_pos, CollisionSize bullet_size, Collision bullet_collision)
	{
		world.each([&](flecs::entity e2, Position& pos, const CollisionSize& size, Collision& collision)
			{
			if (e1.id() == e2.id())
				return;
			if (std::abs(bullet_pos.value.x - pos.value.x) > (bullet_size.value.x + size.value.x) ||
				std::abs(bullet_pos.value.y - pos.value.y) > (bullet_size.value.y + size.value.y) ||
				std::abs(bullet_pos.value.z - pos.value.z) > (bullet_size.value.z + size.value.z))
				return;

			bullet_collision.is_collide = true;
			collision.is_collide = true;
			collision.damage_received = bullet_collision.damage;
			bullet_collision.damage_received = collision.damage;
		});
    });

	world.system<Collision, Health>()
		.each([&](flecs::entity e, Collision collision, Health& health)
		{
		if (!collision.is_collide)
			return;
		health.curr_health -= collision.damage_received;
	});
}