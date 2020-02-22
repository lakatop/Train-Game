#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager* GameManager::Instance()
{
	if (instance == NULL)
		instance = new GameManager();
	return instance;
}

GameManager::GameManager()
{
	graphicsManager = GraphicsManager::Instance();
	FRAME_RATE = 2;
	quit = !(graphicsManager->ReturnSucces());
	timer = Timer::Instance();
}

GameManager::~GameManager()
{
	delete instance;
	instance = NULL;
	Clear();
}

void GameManager::Clear()
{
	timer->Clear();
	graphicsManager->Clear();
}

void GameManager::GameLoop()
{
	while (!quit)
	{
		timer->Update();
		while (SDL_PollEvent(&events) != 0)
		{
			if (events.type == SDL_QUIT)
				quit = true;
		}
		if (timer->GetDelta() >= 1.0f / FRAME_RATE)
		{
			graphicsManager->Render();
			printf("Delta time = %f\n", timer->GetDelta());
			timer->Reset();
		}
	}
}