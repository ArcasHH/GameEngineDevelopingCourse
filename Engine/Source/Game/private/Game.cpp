#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		// How many objects do we want to create
		for (int i = 0; i < 10; ++i)
		{
			int rand_object = std::rand() % 3;
			if (rand_object == 0)
			{
				m_Objects.push_back(new PhysicObject(Math::Vector3f(0.0f, -0.001f * float(i+1), 0.f)));
			}
			else if (rand_object == 1)
			{
				m_Objects.push_back(new MovingObject(Math::Vector3f(float(i), 0.f, float(i*4))));
			}
			else
			{
				m_Objects.push_back(new PlayableObject());
			}
			m_Objects.back()->SetPosition(Math::Vector3f(rand_object * i , rand_object + i, i*i), 0); // object position
			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i]->Update(m_renderThread->GetMainFrame(), dt);
		}
	}
}