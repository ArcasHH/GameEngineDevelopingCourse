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
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 0.0f, -50.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, 0.0f, 1.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();
		std::srand(std::time(0));

		std::shared_ptr<PhysicController> phys_ctr = std::make_shared<PhysicController>(); // for jumping objects
		std::shared_ptr<KeyboardContoller> keyboard_ctr = std::make_shared<KeyboardContoller>(); // for objects with keyboard controll
		std::shared_ptr<MovingController> move_ctr = std::make_shared<MovingController>(); // for moving forward-backward objects
		
		// How many objects do we want to create
		for (int i = 0; i < 100; ++i) 
		{
			int rand_object = std::rand() % 3;
			GameObject* a{};

			if (rand_object == 0)
			{
				a = new GameObject(phys_ctr);
			}
			else if (rand_object == 1)
			{
				a = new GameObject(keyboard_ctr);
			}
			else
			{
				a = new GameObject(move_ctr); 
			}
			// object position
			float rand_radius = float(std::rand() % 50 + 5);
			a->SetPosition(GetRandVec(rand_radius), 0);

			m_Objects.push_back(a);
			Render::RenderObject** renderObject = a->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}
		
		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });

		Core::g_InputHandler->RegisterCallback("MoveRight", [keyboard_ctr](){
				keyboard_ctr->UpdateAll([](GameObject* obj) { 
					obj->SetSpeed(Math::Vector3f(50.f, 0.f, 0.f)); 
					});
			});
		Core::g_InputHandler->RegisterCallback("MoveLeft", [keyboard_ctr]() { 
				keyboard_ctr->UpdateAll([](GameObject* obj) { 
					obj->SetSpeed(Math::Vector3f(-50.f, 0.f, 0.f));
					});
			});
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
		for (const auto& obj : m_Objects)
		{
			obj->Update(m_renderThread->GetMainFrame(), dt);
		}
	}
}

