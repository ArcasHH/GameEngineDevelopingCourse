#pragma once

#include <flecs.h>
#include <Quaternion.h>

namespace GameEngine::Core
{
	class Camera;
	class Controller;
}

struct ControllerPtr
{
	GameEngine::Core::Controller* ptr;
};

struct JumpSpeed
{
	float value;
};

struct CameraPtr
{
	GameEngine::Core::Camera* ptr;
};

struct onRotate
{
	bool isRotate;
	float timer;
	float speed;
};

void RegisterEcsControlSystems(flecs::world& world);

