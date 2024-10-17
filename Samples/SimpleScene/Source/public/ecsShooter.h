#pragma once

#include <flecs.h>

#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN
#endif

void RegisterEcsShooterSystems(flecs::world& world);

ECS_STRUCT(Visibility,
{
	bool is_visible;
});

ECS_STRUCT(Timer,
{
	float timer;
	float max_time;
	bool timer_on;
});

ECS_STRUCT(CollisionSize,
{
	float x;
	float y;
	float z;
	bool is_collide;
});

ECS_STRUCT(ApplyForce,
{
	float x;
	float y;
	float z;
});

ECS_STRUCT(Shot,
{
	float timer;
	float interval;
	bool on_shot;
});

ECS_STRUCT(Bullet,
{
	float damage;
	bool is_bullet;
});