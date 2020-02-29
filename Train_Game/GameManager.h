#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "Timer.h"
#include "LevelManager.h"
#include "TrainComponent.h"
#include "CollectibleItem.h"
#include "NonCollectibleItem.h"
#include "InputManager.h"
#include "Locomotive.h"

class GameManager
{
public:
	static GameManager* Instance();
	void GameLoop();
private:
	GameManager();
	~GameManager();
	void Clear();
	void LoadNewLevel(int levelNumber);
	void CreateComponent(char c, int x, int y);
	void add(std::unique_ptr<Item>);
	void Render();
	void SetInput();
	void Update();
	void CheckCollision();

	bool quit;
	bool explode;
	bool loadNewLevel;
	int FRAME_RATE;
	static GameManager* instance;
	SDL_Event events;
	GraphicsManager* graphicsManager;
	Timer* timer;
	LevelManager* levelManager;
	InputManager* inputManager;
	Locomotive* locomotive;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<TrainComponent>> trainWagons;
};

#endif