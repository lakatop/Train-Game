#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "Timer.h"
#include "LevelManager.h"
#include "TrainComponent.h"
#include "CollectibleItem.h"
#include "NonCollectibleItem.h"
#include "InputManager.h"
#include "Locomotive.h"
#include <map>
#include <set>
#include <algorithm>


class GameManager
{
public:
	static GameManager* Instance();
	void GameLoop();
	~GameManager();
	void Clear();
private:
	GameManager();
	void LoadNewLevel(int levelNumber);
	void CreateComponent(char c, int x, int y);
	void add(std::unique_ptr<Item>);
	void Render();
	void SetInput();
	void Update();
	void UpdateScore();
	bool UpdateFire();
	void UpdateGate();
	void CheckCollision();

	void InitializeScoreBoard();

	bool quit;
	bool explode;
	bool loadNewLevel;
	float FRAME_RATE;
	int SCORE;
	static GameManager* instance;
	SDL_Event events;
	GraphicsManager* graphicsManager;
	Timer* timer;
	LevelManager* levelManager;
	InputManager* inputManager;
	Locomotive* locomotive;
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<TrainComponent>> trainWagons;
	std::map<std::string, int> scoreBoard;
	std::multiset<std::string> basket;
};

#endif