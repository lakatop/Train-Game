#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include "Timer.h"
#include "LevelManager.h"
#include "TrainComponent.h"
#include "CollectibleItem.h"

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
	void add(std::unique_ptr<Component>);

	bool quit;
	bool loadNewLevel;
	int FRAME_RATE;
	static GameManager* instance;
	SDL_Event events;
	GraphicsManager* graphicsManager;
	Timer* timer;
	LevelManager* levelManager;
	std::vector<std::unique_ptr<Component>> components;
};

#endif