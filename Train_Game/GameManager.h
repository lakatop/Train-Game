#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H
#include"GraphicsManager.h"
#include "Timer.h"

class GameManager
{
public:
	static GameManager* Instance();
	void GameLoop();
private:
	GameManager();
	~GameManager();
	void Clear();

	bool quit;
	int FRAME_RATE;
	static GameManager* instance;
	SDL_Event events;
	GraphicsManager* graphicsManager;
	Timer* timer;
};

#endif