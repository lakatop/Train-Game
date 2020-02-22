#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::instance = NULL;

GraphicsManager* GraphicsManager::Instance()
{
	if (instance == NULL)
		instance = new GraphicsManager();
	return instance;
}

GraphicsManager::GraphicsManager()
{
	window = NULL;
	back_buffer = NULL;
	renderer = NULL;
	SCREEN_WIDTH = 1000;
	SCREEN_HEIGT = 800;
	success_initialization = GraphicsManager::Init();
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Clear()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	IMG_Quit();
	SDL_Quit();

	delete instance;
	success_initialization = false;
}

bool GraphicsManager::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL initialization error : %s", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow("Train Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL window creation error : %s", SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("SDL renderer creation error : %s", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	int imgFlags = IMG_INIT_PNG;
	if (!(imgFlags & IMG_Init(imgFlags)))
	{
		printf("SDL image initialization : %s", IMG_GetError());
		return false;
	}
	return true;
}

bool GraphicsManager::ReturnSucces()
{
	return success_initialization;
}

void GraphicsManager::Render()
{
	SDL_RenderPresent(renderer);
}