#pragma once

#include <flecs.h>

#undef ECS_META_IMPL
#ifndef GAME_FRAMEWORK
#define ECS_META_IMPL EXTERN
#endif

ECS_STRUCT(Visibility,
{
	bool is_visible;
});

void RegisterEcsMeshSystems(flecs::world& world);
