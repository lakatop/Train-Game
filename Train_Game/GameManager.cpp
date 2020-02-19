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
	quit = !(graphicsManager->ReturnSucces());
}

GameManager::~GameManager()
{
	//instance->Clear();
	delete instance;
	instance = NULL;
}

void GameManager::GameLoop()
{
	while (!quit)
	{
		while (SDL_PollEvent(&events) != 0)
		{
			if (events.type == SDL_QUIT)
				quit = true;
		}

		graphicsManager->Render();
	}
}