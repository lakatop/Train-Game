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

void Timer::Update()
{
	currentTime = SDL_GetTicks();
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