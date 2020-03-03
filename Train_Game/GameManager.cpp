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
	SCORE = 0;
	explode = false;
	loadNewLevel = true;
	InitializeScoreBoard();
	quit = !(graphicsManager->ReturnSucces());
	timer = Timer::Instance();
	levelManager = LevelManager::Instance();
	inputManager = InputManager::Instance();
	locomotive = NULL;
}

GameManager::~GameManager()
{
}

void GameManager::Clear()
{
	timer->Clear();
	graphicsManager->Clear();
	levelManager->Clear();
	inputManager->Clear();
	locomotive->Clear();
	items.erase(items.begin(), items.end());
	trainWagons.erase(trainWagons.begin(), trainWagons.end());
	delete instance;
	instance = NULL;
}

void GameManager::InitializeScoreBoard()
{
	scoreBoard.insert(std::make_pair("lighter", 1000));
	scoreBoard.insert(std::make_pair("cherry", 500));
	scoreBoard.insert(std::make_pair("tree", 1000));
	scoreBoard.insert(std::make_pair("wine", 750));
	scoreBoard.insert(std::make_pair("rotten_apple", -1000));
}

void GameManager::LoadNewLevel(int levelNumber)
{
	items.erase(items.begin(), items.end());	//clear previous components
	trainWagons.erase(trainWagons.begin(),trainWagons.end());
	locomotive->Clear();
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

void GameManager::add(std::unique_ptr<Item> c)
{
	items.push_back(move(c));
}

void GameManager::CreateComponent(char c, int x, int y)
{
	if (c == 'R') //locomotive
	{
		locomotive = Locomotive::Instance(x,y);
		explode = false;
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
	for (auto it = trainWagons.rbegin(), itend = trainWagons.rend(); it != itend; it++)
	{
		(*it)->SetMoveDirection();
	}
	for (auto&& x : trainWagons)
		x->Update();
}

void GameManager::UpdateScore()
{
	int tempScore = 0;
	for (auto&& x : trainWagons)
	{
		if (!x->GetFire())	// you can increase your score only if the wagon is not on fire, wagons on fire have 0 value
			tempScore += scoreBoard.find(x->GetName())->second;		//else find specific value of an item this wagon is carrying and add it to the score
	}
	SCORE = tempScore;
}

bool GameManager::UpdateFire()
{
	for (auto it = trainWagons.rbegin(), itend = trainWagons.rend(); it != itend; it++)
	{
		if ((*it)->GetFire() && !((*it)->parent->GetFire()))	//find the border between the last burning wagon and the first non-burnig wagon
		{
			(*it)->parent->SetFire();	//set the first non-burning wagon to fire and break(since we want to spread fire only one wagon by one iteration)
			break;
		}
	}
	return locomotive->GetFire();		//return if the locomotive is on fire
}

void GameManager::CheckCollision()
{
	Vector2 lPos = locomotive->GetPosition();
	bool success = false;
	for (auto&& x : trainWagons)	//collision between the locomotive and wagons
	{
		Vector2 tPos = x->GetPosition();
		if (tPos.x == lPos.x && tPos.y == lPos.y)
		{
			explode = true;
			success = true;
			break;
		}
	}

	if (!success)
	{
		int it = -1;
		for (auto&& x : items)	//collision between items and the locomotive
		{
			it++;
			if (x->Collectible() == false)	//either brick or space
			{
				if (x->brick && x->GetPosition() == locomotive->GetPosition() && x->GetDirection() != locomotive->GetDirection())
				{
					explode = true;
					break;
				}
			}
			else  //collectible item, in case of collision attach wagon
			{
				if (x->GetPosition() == locomotive->GetPosition())
				{
					if (trainWagons.empty()) //first wagon, its parent = locomotive
					{
						Vector2 tPos = locomotive->GetPreviousPosition();
						trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(),locomotive->GetDirection(),locomotive));
					}
					else
					{
						Vector2 tPos = trainWagons.back()->GetPreviousPosition();
						trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(),
							trainWagons.back()->GetPreviousMoveDirection(), trainWagons.back()->GetPointer()));	//calling GetPreviousMoveDirection for correct wagon drawing
						trainWagons.back()->CheckFireCollision();	// check if there isnt collision between wood and lighter that would trigger fire
					}
					items.erase(items.begin() + it);	//delete item that now represents wagon
					break;
				}
			}
		}
	}

	if (trainWagons.empty() == false)	//boundary case when locomotive turns itself against its wagons
	{
		if (trainWagons[0]->GetPreviousPosition() == locomotive->GetPosition())
		{
			explode = true;
		}
	}
}

void GameManager::Render()
{
	for (auto&& x : items)
		x->Render();
	locomotive->Render();
	for (auto&& x : trainWagons)
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
			{
				Update();
				CheckCollision();
				if (explode || UpdateFire())
					quit = true;
				UpdateScore();
			}
			graphicsManager->Render();
			Render();
			SDL_RenderPresent(graphicsManager->GetRenderer());
			printf("SCORE : %d\n", SCORE);
			timer->Reset();
		}
	}
}