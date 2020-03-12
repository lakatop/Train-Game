/*
Peter Lakatos
Timer.h
Train_Game
*/
#ifndef _TIMER_H
#define _TIMER_H

#include <SDL.h>

class Timer
{
public:
	void Update();
	void Reset();
	float GetDelta();
	void Clear();

	static Timer* Instance();

private:
	Timer();
	~Timer();

	static Timer* instance;
	float deltaTime;
	unsigned int currentTime;
	unsigned int startTime;
};

#endif
