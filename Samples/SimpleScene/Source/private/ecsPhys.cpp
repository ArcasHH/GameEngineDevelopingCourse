#include <ecsPhys.h>
#include <ecsShooter.h>
#include <flecs.h>

namespace
{
	inline float rand_flt(float from, float to)
	{
		return from + (float(rand()) / RAND_MAX) * (to - from);
	}
}

void RegisterEcsPhysSystems(flecs::world& world)
{
	world.system< Velocity, const Gravity, BouncePlane*, Position*>()
		.each([&](flecs::entity e, Velocity& vel, const Gravity& grav, BouncePlane* plane, Position* pos)
	{
		if (plane && pos)
		{
			constexpr float planeEpsilon = 0.1f;

			if (plane->value.x * pos->value.x + plane->value.y * pos->value.y + plane->value.z * pos->value.z < plane->value.w + planeEpsilon)
			{
				return;
			}
		}
		vel.value.x += grav.value.x * world.delta_time();
		vel.value.y += grav.value.y * world.delta_time();
		vel.value.z += grav.value.z * world.delta_time();
	});


	world.system<Velocity, Position, const BouncePlane, const Bounciness>()
		.each([&](Velocity& vel, Position& pos, const BouncePlane& plane, const Bounciness& bounciness)
	{
		float dotPos = plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z;
		float dotVel = plane.value.x * vel.value.x + plane.value.y * vel.value.y + plane.value.z * vel.value.z;
		if (dotPos < plane.value.w)
		{
			pos.value.x -= (dotPos - plane.value.w) * plane.value.x;
			pos.value.y -= (dotPos - plane.value.w) * plane.value.y;
			pos.value.z -= (dotPos - plane.value.w) * plane.value.z;

			vel.value.x -= (1.f + bounciness.value) * plane.value.x * dotVel;
			vel.value.y -= (1.f + bounciness.value) * plane.value.y * dotVel;
			vel.value.z -= (1.f + bounciness.value) * plane.value.z * dotVel;
		}
	});


	world.system<Velocity, const FrictionAmount>()
		.each([&](flecs::entity e, Velocity& vel, const FrictionAmount& friction)
	{
		vel.value.x -= vel.value.x * friction.value * world.delta_time();
		vel.value.y -= vel.value.y * friction.value * world.delta_time();
		vel.value.z -= vel.value.z * friction.value * world.delta_time();
	});


	world.system<Position, const Velocity>()
		.each([&](flecs::entity e, Position& pos, const Velocity& vel)
	{
		pos.value.x += vel.value.x * world.delta_time();
		pos.value.y += vel.value.y * world.delta_time();
		pos.value.z += vel.value.z * world.delta_time();
	});


	world.system<Position, const ShiverAmount>()
		.each([&](flecs::entity e, Position& pos, const ShiverAmount& shiver)
	{
		pos.value.x += rand_flt(-shiver.value, shiver.value);
		pos.value.y += rand_flt(-shiver.value, shiver.value);
		pos.value.z += rand_flt(-shiver.value, shiver.value);
	});

	world.system<Position, CollisionSize, Collision, Bullet>()
		.each([&](flecs::entity e1, Position& bullet_pos,const CollisionSize& bullet_size, Collision& bullet_collision, Bullet&_)
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
		});
    });
}
