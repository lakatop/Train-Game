#include "Timer.h"

Timer* Timer::instance = NULL;

Timer* Timer::Instance()
{
	if (instance == NULL)
		instance = new Timer();
	return instance;
}

Timer::Timer()
{
	startTime = SDL_GetTicks();
	currentTime = 0;
	deltaTime = 0;
}

Timer::~Timer()
{
}

//update is called every while loop iteration(game loop iteration)
//it is basically just increasing delta time, until Reset() is called
void Timer::Update()
{
	currentTime = SDL_GetTicks();
	//deltaTime represents seconds between current time and startTime
	deltaTime = (currentTime - startTime) * 0.001f;
}

void Timer::Reset()
{
	startTime = currentTime;
	deltaTime = 0.0f;
}

float Timer::GetDelta()
{
	return deltaTime;
}

void Timer::Clear()
{
	delete instance;
	instance = NULL;
}