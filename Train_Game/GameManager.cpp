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
	inputManager = InputManager::Instance();
	locomotive = NULL;
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
	inputManager->Clear();
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
		locomotive = Locomotive::Instance(x,y);
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
		add(std::make_unique<NonCollectibleItem>(x, y, "brick", true));
	}
	else if (c == '<') //left_brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "left_brick", true,Vector2(-1,0)));
	}
	else if (c == '^') //up_brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "up_brick", true, Vector2(0, -1)));
	}
	else if (c == '>') //right_brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "right_brick", true, Vector2(1, 0)));
	}
	else if (c == 'v') //down_brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "down_brick", true, Vector2(0, 1)));
	}
	else if (c == ' ') //empty space
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "space"));
	}
	else
	{
		printf("Error : unknown item");
		exit(0);
	}
}

void GameManager::SetInput()
{
	if (inputManager->KeyDown(SDL_SCANCODE_UP))
	{
		locomotive->moving = true;
		locomotive->SetMoveDirection(Vector2(0, -1));
	}
	else if (inputManager->KeyDown(SDL_SCANCODE_DOWN))
	{
		locomotive->moving = true;
		locomotive->SetMoveDirection(Vector2(0, 1));
	}
	else if (inputManager->KeyDown(SDL_SCANCODE_RIGHT))
	{
		locomotive->moving = true;
		locomotive->SetMoveDirection(Vector2(1, 0));
	}
	else if (inputManager->KeyDown(SDL_SCANCODE_LEFT))
	{
		locomotive->moving = true;
		locomotive->SetMoveDirection(Vector2(-1, 0));
	}
}

void GameManager::Update()
{
	locomotive->Update();
	for (auto it = trainWagons.begin(), itEnd = trainWagons.end(); it != itEnd; it++)
	{
		(*it)->Update();
	}
}

void GameManager::Render()
{
	locomotive->Render();
	for (auto&& x : components)
		x->Render();
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
		inputManager->Update();
		SetInput();
		if (timer->GetDelta() >= 1.0f / FRAME_RATE)
		{
			SDL_RenderClear(graphicsManager->GetRenderer());
			if (locomotive != NULL && locomotive->moving)
				Update();
			graphicsManager->Render();
			Render();
			SDL_RenderPresent(graphicsManager->GetRenderer());
			printf("Delta time = %f\n", timer->GetDelta());
			timer->Reset();
		}
	}
}