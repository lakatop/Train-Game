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
	renderRect = { 0,0,0,0 };
	SCREEN_WIDTH = 1000;
	SCREEN_HEIGT = 800;
	IMAGE_SIZE = 50;
	FONT_SIZE = 20;
	SetFlipAngleArray();
	success_initialization = GraphicsManager::Init();
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Clear()
{
	for (auto&& x : text)
		if (x.second != NULL)
			SDL_DestroyTexture(x.second);
	text.clear();

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	delete instance;
	instance = NULL;
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
		printf("SDL image initialization error : %s", IMG_GetError());
		return false;
	}
	if (TTF_Init() < 0)
	{
		printf("SDL TTF initialization error : %s", TTF_GetError());
		return false;
	}
	return true;
}

bool GraphicsManager::ReturnSucces()
{
	return success_initialization;
}

double GraphicsManager::GetFlipAngle(int x, int y)
{
	if (x == -1 && y == 0)	//left direction : just flip horizontally
		flip = SDL_FLIP_HORIZONTAL;
	else
		flip = SDL_FLIP_NONE;
	return flipArray[x + 1][y + 1];
}

void GraphicsManager::SetFlipAngleArray()
{
	/*
	direction vectors : (-1,0), (1,0), (0,1), (0,-1) 
	to each vector add 1 (because of the negative numbers)
	and that will be an index in the flipArray that holds which angle to pass*/
	flipArray[0][1] = 0;
	flipArray[2][1] = 0;
	flipArray[1][2] = 90;
	flipArray[1][0] = -90;
}

SDL_Texture* GraphicsManager::GetText(std::string path)
{
	if (auto it = text.find(path) == text.end())
		text.insert(std::make_pair(path, CreateText(path)));

	SDL_QueryTexture(text.find(path)->second, NULL, NULL, &renderRect.w, &renderRect.h);	//set width and height of new texture to renderRect for later rendering
	
	return text.find(path)->second;
}

SDL_Texture* GraphicsManager::CreateText(std::string text)
{
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("/Art/Retro Gaming.ttf");
	TTF_Font* font = TTF_OpenFont(fullPath.c_str(),FONT_SIZE);
	if (font == NULL)
	{
		printf("Font creating error : %s", TTF_GetError());
		return NULL;
	}
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), { 255,255,255 });
	if (surface == NULL)
	{
		printf("Text render error : %s", TTF_GetError());
		return NULL;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == NULL)
	{
		printf("Text texture creating error : %s", SDL_GetError());
		return NULL;
	}
	SDL_FreeSurface(surface);
	return tex;
}

void GraphicsManager::Render(std::string path, int height)
{
	renderRect.y = height + FONT_SIZE;
	SDL_RenderCopy(renderer, GetText(path), NULL, &renderRect);
	SDL_RenderPresent(renderer);
}

SDL_Renderer* GraphicsManager::GetRenderer()
{
	return renderer;
}