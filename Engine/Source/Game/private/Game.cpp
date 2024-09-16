#include <Game.h>

#include <chrono>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		m_renderEngine = std::make_unique<Render::RenderEngine>();
		
		ProcessSystemParams();
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		auto c_start = std::chrono::system_clock::now();
		bool quit = false;
		while (!quit)
		{
			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();

			ProcessSystemParams();

			auto c_end = std::chrono::system_clock::now();
			auto diff = std::chrono::duration<float>(c_end - c_start);
			float dt = diff.count();	
			c_start = c_end;
			
			m_renderEngine->Update(dt);
		}
	}

	void Game::ProcessSystemParams()
	{
		if (m_renderEngine->GetSwapChainWidth() != Core::MainWindowsApplication->GetWidth() ||
			m_renderEngine->GetSwapChainHeight() != Core::MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			m_renderEngine->OnResize(Core::MainWindowsApplication->GetWidth(), Core::MainWindowsApplication->GetHeight());
		}
	}
}