/*
Peter Lakatos
GameManager.h
Train_Game
*/
#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include "Timer.h"
#include "LevelManager.h"
#include "TrainComponent.h"
#include "CollectibleItem.h"
#include "NonCollectibleItem.h"
#include "InputManager.h"
#include "AudioManager.h"
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
	void LoadNewLevel(const int levelNumber);
	void CreateComponent(const char c, const int x, const int y);
	void add(std::unique_ptr<Item>);
	void Render();
	void SetInput();
	void Update();
	void UpdateScore();
	bool UpdateFire();
	void UpdateGate();
	void CheckCollision();
	bool CheckLevelSuccess();
	void PlayMusic();
	void RenderSpecialScreen();
	void InitializeScoreBoard();
	
	static GameManager* instance;
	
	bool quit;
	bool explode;
	bool levelSuccess;
	bool loadNewLevel;
	bool renderExplosion;
	bool musicPlaying;
	float FRAME_RATE;
	int SCORE;
	SDL_Event events;
	
	GraphicsManager* graphicsManager;
	Timer* timer;
	LevelManager* levelManager;
	InputManager* inputManager;
	AudioManager* audioManager;
	Locomotive* locomotive;
	
	std::vector<std::unique_ptr<Item>> items;
	std::vector<std::unique_ptr<TrainComponent>> trainWagons;
	std::map<std::string, int> scoreBoard;
	std::multiset<std::string> basket;
};

#endif