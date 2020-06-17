#include "GameManager.hpp"

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
	timer = Timer::Instance();
	levelManager = LevelManager::Instance();
	inputManager = InputManager::Instance();
	audioManager = AudioManager::Instance();
	
	events.type = NULL;
	FRAME_RATE = 2;
	SCORE = 0;
	explode = false;
	loadNewLevel = false;
	levelSuccess = false;
	renderExplosion = false;
	musicPlaying = false;
	InitializeScoreBoard();
	quit = !(graphicsManager->ReturnSuccess());
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
	scoreBoard.erase(scoreBoard.begin(), scoreBoard.end());

	delete instance;
	instance = NULL;
}

void GameManager::InitializeScoreBoard()
{
	//set score to all components int the game
	scoreBoard.insert({ "lighter", 1000 });
	scoreBoard.insert({ "cherry", 500 });
	scoreBoard.insert({ "tree", 1000 });
	scoreBoard.insert({ "wine", 750 });
	scoreBoard.insert({ "rotten_apple", -1000 });
	scoreBoard.insert({ "final", 5000 });
}

void GameManager::LoadNewLevel(const int levelNumber)
{
	//stop the music that is playing from some load screen
	audioManager->StopMusic();
	musicPlaying = false;

	//clear previous components
	items.erase(items.begin(), items.end());
	trainWagons.erase(trainWagons.begin(),trainWagons.end());
	basket.erase(basket.begin(), basket.end());
	if(locomotive != NULL)
		locomotive->Clear();

	//get width and height of current level
	int width = levelManager->GetLevelWidth(levelNumber);
	int height = levelManager->GetLevelHeight(levelNumber);

	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			char c = levelManager->GetLevelChar(h, w, levelNumber);	//get current character...
			CreateComponent(c, w, h);								//...and create component 
		}
	}

	loadNewLevel = false;	//level is successfully loaded, no need to load new level now
	
	//fill up the basket with all collectible items
	for (auto&& x : items)
	{
		if (x->Collectible() && x->GetName() != "rotten_apple")
			basket.insert(x->GetName());
	}

	//check if there is locomotive
	if (locomotive == NULL)
	{
		printf("Error : level without locomotive");
		exit(0);
	}
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
	else if (c == 'F')	//final item
	{
		add(std::make_unique<CollectibleItem>(x, y, "final"));
	}
	else if (c == 'B') //brick
	{
		add(std::make_unique<NonCollectibleItem>(x, y, "brick", true));
	}
	else if (c == 'G')	//gate
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
	//if user pushed esc button -> quit game
	if (inputManager->KeyDown(SDL_SCANCODE_ESCAPE))
	{
		quit = true;
	}

	//no explosion, no finished level, no start screen, locomotive started moving -> set its moving direction
	if (!explode && !levelSuccess && locomotive != NULL)
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
	{	//some special screen is being rendered
		//if user pushed Enter button -> load some level
		if (inputManager->KeyDown(SDL_SCANCODE_RETURN))
		{
			//start of the game, user pushed enter to start playing
			if (locomotive == NULL && !loadNewLevel)
			{
				loadNewLevel = true;
			}
			//exploded, restart actual level
			else if (explode)
			{
				SCORE = 0;
				explode = false;
				LoadNewLevel(levelManager->actualLevel);
			}
			//won the game, restart the whole game
			else if (levelSuccess && levelManager->actualLevel == levelManager->GetLevelCount())
			{
				SCORE = 0;
				levelSuccess = false;
				levelManager->actualLevel = 1;
				LoadNewLevel(1);
			}
			//successfully finished level, load next level
			else  
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
	//update position of a locomotive and train wagons
	locomotive->Update();
	for (auto it = trainWagons.rbegin(), itend = trainWagons.rend(); it != itend; it++)
	{
		(*it)->SetMoveDirection();	//get move direction from parent
	}
	for (auto&& x : trainWagons)
		x->Update();

	//check if locomotive exploded because of fire
	explode = UpdateFire();
	
	//check if player collected all collectible items -> open gate
	UpdateGate();

	//check if locomotive is standing on open gate
	levelSuccess = CheckLevelSuccess();
}

void GameManager::UpdateScore()
{
	SCORE = 0;
	for (auto&& x : trainWagons)
	{
		if (!x->GetFire())	// score can be increased only if the wagon is not on fire, wagons on fire have 0 value
			SCORE += scoreBoard.find(x->GetName())->second;		//find specific value of an item this wagon is carrying and add it to the score
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
	return locomotive->GetFire();		//return condition of locomotive
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
	bool success = false;

	//collision between the locomotive and wagons
	for (auto&& x : trainWagons)
	{
		if (x->GetPosition() == locomotive->GetPosition())
		{
			explode = true;
			success = true;
			break;
		}
	}

	if (!success)
	{
		int it = -1;

		//collision between items and the locomotive
		for (auto&& x : items)
		{
			it++;
			if (x->Collectible() == false)	//either brick or space
			{
				if (x->brick && x->GetPosition() == locomotive->GetPosition() && x->GetDirection() != locomotive->GetDirection())	//direction is being checked because of special bricks with arrow
				{
					explode = true;
					break;
				}
			}
			else  //collectible item, in case of collision attach wagon
			{
				if (trainWagons.empty() || (!trainWagons.empty() && !trainWagons.back()->GetLast())) //check if the last wagon isnt carrying "final" item, if yes, locomotive cant pick any more items
				{
					if (x->GetPosition() == locomotive->GetPosition())
					{
						bool last = (x->GetName() == "final") ? true : false;	//if this item represents "final item", than set true, otherwise false
						if (trainWagons.empty()) //first wagon, its parent = locomotive
						{
							Vector2 tPos = locomotive->GetPreviousPosition();
							trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(),
								locomotive->GetDirection(), last, locomotive));
						}		
						else
						{
							Vector2 tPos = trainWagons.back()->GetPreviousPosition();
							trainWagons.emplace_back(std::make_unique<TrainComponent>(tPos.x, tPos.y, x->GetName(),
								trainWagons.back()->GetPreviousMoveDirection(), last, trainWagons.back()->GetPointer()));	//calling GetPreviousMoveDirection for correct wagon drawing
							trainWagons.back()->CheckFireCollision();	// check if there isnt collision between wood and lighter that would trigger fire
						}
						if(x->GetName() != "rotten_apple")
							basket.erase(basket.lower_bound(x->GetName()));	//delete item that now represents wagon from basket...
						items.erase(items.begin() + it);	//... and also delete it from items
						break;
					}
				}
			}
		}
	}

	//boundary case when locomotive turns itself against its wagons
	if (trainWagons.empty() == false)
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
	//in case that train(locomotive) exploded, set it to position where it was before an explosion because of correct rendering
	if (renderExplosion)
	{
		for (auto&& x : trainWagons)
			x->SetToPreviousPosition();
		locomotive->SetToPreviousPosition();
	}
	
	//render all items
	for (auto&& x : items)
		x->Render();
	
	//if locomotive didnt explode, render it
	if (!explode)
		locomotive->Render();
	
	//render all train wagons
	for (auto&& x : trainWagons)
		x->Render();
	
	//render score
	graphicsManager->RenderScore("Score : " + std::to_string(SCORE), 
								levelManager->GetLevelHeight(levelManager->actualLevel) * graphicsManager->SetPictureSize() + graphicsManager->GetHeightOffset());
	
	//if locomotive exploded, and explosion wasn't rendered yet, render it
	if (renderExplosion)
	{
		audioManager->PlayEffect("explode");
		graphicsManager->RenderExplosion(locomotive->GetPosition());
		renderExplosion = false;
	}
}

void GameManager::RenderSpecialScreen()
{
	//render explode screen
	if (explode)
		graphicsManager->RenderSpecialScreen("ExplodeScreen");
	//successfully finished level and current level = final level -> player won -> render win screen
	else if (levelSuccess && levelManager->actualLevel == levelManager->GetLevelCount())
	{
		PlayMusic();
		graphicsManager->RenderSpecialScreen("WinScreen");
	}
	//locomotive is NULL -> level wasn't loaded yet -> render start screen
	else if (locomotive == NULL)
	{
		PlayMusic();
		graphicsManager->RenderSpecialScreen("StartScreen");
	}
	//player sucessfully finished level, but it wasn't final level -> render finished level screen
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
		//load new level
		if (loadNewLevel)
		{
			levelManager->actualLevel++;
			LoadNewLevel(levelManager->actualLevel);
		}

		//update timer
		timer->Update();

		//check if user didnt close window
		while (SDL_PollEvent(&events) != 0)
		{
			if (events.type == SDL_QUIT)
				quit = true;
		}

		//update users input
		inputManager->Update();
		SetInput();
		
		//handle other situations every 0.5 second
		if (timer->GetDelta() >= 1.0f / FRAME_RATE)
		{
			//clear screen from previous rendering
			SDL_RenderClear(graphicsManager->GetRenderer());

			//no special screen, locomotive is moving and didn't explode yet
			if (locomotive != NULL && locomotive->moving && !explode && !levelSuccess)
			{
				//play "move" sound effect
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
			
			timer->Reset();
		}
	}
}