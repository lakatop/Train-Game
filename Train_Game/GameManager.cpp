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
	events.type = SDL_QUIT;
	FRAME_RATE = 2;
	loadNewLevel = true;
	quit = !(graphicsManager->ReturnSucces());
	timer = Timer::Instance();
	levelManager = LevelManager::Instance();
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
	levelManager->Clear();
}

void GameManager::LoadNewLevel(int levelNumber)
{
	for (auto&& x : components)	//clear previous components
		x = nullptr;
	int width = levelManager->GetLevelWidth(levelNumber);
	int height = levelManager->GetLevelHeight(levelNumber);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			char c = levelManager->GetLevelChar(h, w, levelNumber);
			CreateComponent(c, w, h);
		}
	}
	loadNewLevel = false;
}

void GameManager::add(std::unique_ptr<Component> c)
{
	components.push_back(move(c));
}

void GameManager::CreateComponent(char c, int x, int y)
{
	if (c == 'R') //locomotive
	{
		add(std::make_unique<TrainComponent>(x, y, true, "locomotive"));
	}
	else if (c == 'L') //lighter
	{
		add(std::make_unique<CollectibleItem>(x, y, "lighter"));
	}
	else if (c == 'C') //cherry
	{
		add(std::make_unique<CollectibleItem>(x, y, "cherry"));
	}
	else if (c == 'T') // tree
	{
		add(std::make_unique<CollectibleItem>(x, y, "tree"));
	}
	else if (c == 'W') //wine
	{
		add(std::make_unique<CollectibleItem>(x, y, "wine"));
	}
	else if (c == 'A') // rotten apple
	{
		add(std::make_unique<CollectibleItem>(x, y, "rotten_apple"));
	}
	else if (c == 'B') //brick
	{

	}
	else if (c == '<') //left_brick
	{

	}
	else if (c == '^') //up_brick
	{

	}
	else if (c == '>') //right_brick
	{

	}
	else if (c == 'v') //down_brick
	{

	}
	else if (c == ' ') //empty space
	{

	}
	else
	{
		printf("Error : unknown item");
		exit(0);
	}
}

void GameManager::GameLoop()
{
	while (!quit)
	{
		if (loadNewLevel)
		{
			LoadNewLevel(levelManager->GetActualLevel());
		}
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