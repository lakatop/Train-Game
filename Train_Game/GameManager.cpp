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
	loadNewLevel = false;
	levelSuccess = false;
	renderExplosion = false;
	musicPlaying = false;
	InitializeScoreBoard();
	quit = !(graphicsManager->ReturnSucces());
	timer = Timer::Instance();
	levelManager = LevelManager::Instance();
	inputManager = InputManager::Instance();
	audioManager = AudioManager::Instance();
	locomotive = NULL;
}

GameManager::~GameManager()
{
}

void GameManager::Clear()
{
	timer->Clear();
	audioManager->Clear();
	graphicsManager->Clear();
	levelManager->Clear();
	inputManager->Clear();
	locomotive->Clear();
	
	items.erase(items.begin(), items.end());
	trainWagons.erase(trainWagons.begin(), trainWagons.end());
	basket.erase(basket.begin(), basket.end());
	
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
	scoreBoard.insert(std::make_pair("final", 5000));
}

void GameManager::LoadNewLevel(const int levelNumber)
{
	audioManager->StopMusic();
	musicPlaying = false;
	items.erase(items.begin(), items.end());	//clear previous components
	trainWagons.erase(trainWagons.begin(),trainWagons.end());
	basket.erase(basket.begin(), basket.end());
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
	for (auto&& x : items)	//fill up the basket with all collectible items
	{
		if (x->Collectible())
			basket.insert(x->GetName());
	}
	locomotive->GetDirection();
}

void GameManager::add(std::unique_ptr<Item> c)
{
	items.push_back(move(c));
}

void GameManager::CreateComponent(const char c, const int x, const int y)
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
	else if (c == 'F')
	{
		add(std::make_unique<CollectibleItem>(x, y, "final"));
	}
	else if (c == 'B') //brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "brick", true));
	}
	else if (c == 'G')
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "gate", true));
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
	if (inputManager->KeyDown(SDL_SCANCODE_ESCAPE))
	{
		quit = true;
	}
	if (!explode && !levelSuccess && locomotive != NULL)	//no exposion, no finished level, no start screen
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
	else
	{
		if (inputManager->KeyDown(SDL_SCANCODE_RETURN))
		{
			if (locomotive == NULL && !loadNewLevel)	//start of the game, user pushed enter to start playing
			{
				loadNewLevel = true;
			}
			else if (explode)	//exploded, restart actual level
			{
				SCORE = 0;
				explode = false;
				LoadNewLevel(levelManager->actualLevel);
			}
			else if (levelSuccess && levelManager->actualLevel == levelManager->GetLevelCount()) //won the game, restart the whole game
			{
				SCORE = 0;
				levelSuccess = false;
				levelManager->actualLevel = 1;
				LoadNewLevel(1);
			}
			else   //successfully finished level, load next level
			{
				SCORE = 0;
				levelSuccess = false;
				loadNewLevel = true;
			}
		}
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
	explode = UpdateFire();
	UpdateGate();
	levelSuccess = CheckLevelSuccess();
}

void GameManager::UpdateScore()
{
	SCORE = 0;
	for (auto&& x : trainWagons)
	{
		if (!x->GetFire())	// you can increase your score only if the wagon is not on fire, wagons on fire have 0 value
			SCORE += scoreBoard.find(x->GetName())->second;		//else find specific value of an item this wagon is carrying and add it to the score
	}
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

void GameManager::UpdateGate()
{
	if (basket.empty())	//all collectible items are picked up, now i can open the gate
	{
		int pos = -1;
		for (auto&& x : items)
		{
			pos++;
			if (x->GetName() == "gate")
			{
				Vector2 v = (*(items.begin() + pos))->GetPosition();	// get gate's position
				items.erase(items.begin() + pos);	//delete gate from items ...
				add(std::make_unique<NonCollectibleItem>(v.x, v.y, "open_gate"));	//... and replace it by open gate
				break;
			}
		}
	}
}

bool GameManager::CheckLevelSuccess()
{
	/*
	i know that when gate is open, it is added as the last item,
	because i am adding to "items" only when loading level, and then once more when adding "open_gate"
	*/
	if (items.back()->GetName() == "open_gate")	//if true -> gate is opened
	{
		if (items.back()->GetPosition() == locomotive->GetPosition())	//if true -> locomotive has successfully reached final destination with all items picked up
			return true;
	}
	return false;
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
				if (trainWagons.empty() || (!trainWagons.empty() && !trainWagons.back()->GetLast())) //check if the last wagon isnt carrying "final" item, if yes, you cant pick any more items
				{
					if (x->GetPosition() == locomotive->GetPosition())
					{
						bool last = (x->GetName() == "final") ? true : false;	//if this item represents "final" item, than set true, otherwise false
						if (trainWagons.empty()) //first wagon, its parent = locomotive
						{
							Vector2 tPos = locomotive->GetPreviousPosition();
							trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(), locomotive->GetDirection(), last, locomotive));
						}
						else
						{
							Vector2 tPos = trainWagons.back()->GetPreviousPosition();
							trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(),
								trainWagons.back()->GetPreviousMoveDirection(), last, trainWagons.back()->GetPointer()));	//calling GetPreviousMoveDirection for correct wagon drawing
							trainWagons.back()->CheckFireCollision();	// check if there isnt collision between wood and lighter that would trigger fire
						}
						basket.erase(basket.lower_bound(x->GetName()));	//delete item that now represents wagon from basket...
						items.erase(items.begin() + it);	//... and also delete it from items
						break;
					}
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

void GameManager::PlayMusic()
{
	if (!musicPlaying)
	{
		audioManager->PlayMusic("win");
		musicPlaying = true;
	}
}

void GameManager::Render()
{
	if (renderExplosion)	//in case that train(locomotive) exploded, set it to position where it was before an explosion
	{
		for (auto&& x : trainWagons)
			x->SetToPreviousPosition();
		locomotive->SetToPreviousPosition();
	}
	for (auto&& x : items)
		x->Render();
	if (!explode)
		locomotive->Render();
	for (auto&& x : trainWagons)
		x->Render();
	graphicsManager->Render("Score : " + std::to_string(SCORE), 
		levelManager->GetLevelHeight(levelManager->actualLevel) * graphicsManager->SetPictureSize() + graphicsManager->GetHeightOffset());
	if (renderExplosion)
	{
		audioManager->PlayEffect("explode");
		graphicsManager->RenderExplosion(locomotive->GetPosition());
		renderExplosion = false;
	}
}

void GameManager::RenderSpecialScreen()
{
	if (explode)
		graphicsManager->RenderSpecialScreen("ExplodeScreen");
	else if (levelSuccess && levelManager->actualLevel == levelManager->GetLevelCount())
	{
		PlayMusic();
		graphicsManager->RenderSpecialScreen("WinScreen");
	}
	else if (locomotive == NULL)
	{
		PlayMusic();
		graphicsManager->RenderSpecialScreen("StartScreen");
	}
	else if (levelSuccess)
	{
		PlayMusic();
		graphicsManager->RenderSpecialScreen("LevelScreen");
	}
}

void GameManager::GameLoop()
{
	while (!quit)
	{
		if (loadNewLevel)
		{
			levelManager->actualLevel++;
			LoadNewLevel(levelManager->actualLevel);
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
			if (locomotive != NULL && locomotive->moving && !explode && !levelSuccess)
			{
				audioManager->PlayEffect("move");
				Update();
				CheckCollision();
				UpdateScore();
				renderExplosion = (explode && !renderExplosion) ? true : false;		//if it just exploded, render explosion
			}
			if(locomotive != NULL)	//render only if there is no start screen
				Render();
			RenderSpecialScreen();
			SDL_RenderPresent(graphicsManager->GetRenderer());
			printf("basket size : %d\n", basket.size());
			timer->Reset();
		}
	}
}