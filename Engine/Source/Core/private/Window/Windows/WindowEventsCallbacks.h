#pragma once

#include <Camera.h>
#include <Constants.h>
#include <Window/IWindow.h>
#include <Window.h>

#include <Input.h>

namespace GameEngine::Core
{
    void OnMouseDown(WPARAM btnState, int x, int y, Window* window)
    {
        window->SetMousePos(x, y);

        SetCapture(GetPlatformWindowHandle(window->GetWindowHandle()));
    }

    void OnMouseUp(WPARAM btnState, int x, int y)
    {
        ReleaseCapture();
    }

    void OnMouseMove(WPARAM btnState, int x, int y, Camera* camera, Window* window)
    {
        Math::Vector2i mousePos = window->GetMousePos();

        if ((btnState & MK_LBUTTON) != 0)
        {
            float dx = 0.25 * static_cast<float>(x - mousePos.x) * Math::Constants::PI / 180.f;
            float dy = 0.25 * static_cast<float>(y - mousePos.y) * Math::Constants::PI / 180.f;

            dy = -dy; // To avoid inverse movement
            camera->Rotate(dx, dy);
        }
        else if ((btnState & MK_RBUTTON) != 0)
        {
            float dx = 0.05f * static_cast<float>(x - mousePos.x);
            float dy = 0.05f * static_cast<float>(y - mousePos.y);

            Math::Vector3f offset = camera->GetViewDir() * (dx - dy);

            Math::Vector3f position = camera->GetPosition();
            position = position + offset;

            camera->SetPosition(position);
        }

        window->SetMousePos(x, y);
    }

    inline void OnKeyPress(WPARAM btnState, Camera* camera, Window* window)
    {
        const Keyboard &Keys = getKeyboard();
        ActionOnPressed Action = Keys.getAction(btnState);

        float speed = 0.1;
        Math::Vector3f forward = camera->GetViewDir();
        Math::Vector3f up = Math::Vector3f(0.0f, 1.0f, 0.0f);
        Math::Vector3f right = forward.CrossProduct(up).Normalized();

        switch (Action)
        {
        case ActionOnPressed::FORWARD:
            camera->SetSpeed(camera->GetSpeed() + forward * speed);
            break;
        case ActionOnPressed::BACKWARD:
            camera->SetSpeed(camera->GetSpeed() - forward * speed);
            break;
        case ActionOnPressed::LEFT:
            camera->SetSpeed(camera->GetSpeed() + right * speed);
            break;
        case ActionOnPressed::RIGHT:
            camera->SetSpeed(camera->GetSpeed() - right * speed);
            break;
        case ActionOnPressed::UNKNOWN:
            break;
        default:
            break;
        }
    }
}

