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

SDL_Texture* GraphicsManager::SetTexture(const std::string path, const std::string name)
{
	SDL_Texture* tex = NULL;
	std::string fullPath = path + "Art/" + name + ".png";
	SDL_Surface* surface = IMG_Load(fullPath.c_str());
	if (surface == NULL)
	{
		printf("Error loading image : %s\n", IMG_GetError());
		exit(0);
	}
	tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == NULL)
	{
		printf("Error creating texture : %s\n", SDL_GetError());
		exit(0);
	}
	SDL_FreeSurface(surface);
	return tex;
}

bool GraphicsManager::ReturnSucces()
{
	return success_initialization;
}

int GraphicsManager::SetPictureSize()
{
	int screenWidth = 0;
	int screenHeigt = 0;
	SDL_GetWindowSize(window, &screenWidth, &screenHeigt);
	int sizeWidth = screenWidth / LevelManager::Instance()->GetLevelWidth(LevelManager::Instance()->actualLevel);
	int sizeHeight = screenHeigt / LevelManager::Instance()->GetLevelHeight(LevelManager::Instance()->actualLevel);
	return std::min(sizeWidth, sizeHeight);
}

int GraphicsManager::GetHeightOffset()
{
	int levelHeight = SetPictureSize() * LevelManager::Instance()->GetLevelHeight(LevelManager::Instance()->actualLevel);
	int screenHeight = 0;
	SDL_GetWindowSize(window, NULL, &screenHeight);
	return (screenHeight - levelHeight) / 2;
}

int GraphicsManager::GetWidthOffSet()
{
	int levelWidth = SetPictureSize() * LevelManager::Instance()->GetLevelWidth(LevelManager::Instance()->actualLevel);
	int screenWidth = 0;
	SDL_GetWindowSize(window, &screenWidth, NULL);
	return (screenWidth - levelWidth) / 2;
}

double GraphicsManager::GetFlipAngle(const int x, const int y)
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

SDL_Texture* GraphicsManager::GetText(const std::string path)
{
	if (text.find(path) == text.end())
		text.insert(std::make_pair(path, CreateText(path)));

	SDL_QueryTexture(text.find(path)->second, NULL, NULL, &renderRect.w, &renderRect.h);	//set width and height of new texture to renderRect for later rendering
	
	return text.find(path)->second;
}

SDL_Texture* GraphicsManager::CreateText(const std::string text)
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

void GraphicsManager::Render(const std::string path, const int height)
{
	//render SCORE
	renderRect.y = height + FONT_SIZE;
	SDL_RenderCopy(renderer, GetText(path), NULL, &renderRect);
}

void GraphicsManager::RenderSpecialScreen(std::string name)
{
	SDL_Texture* tex = SetTexture(SDL_GetBasePath(), name);
	SDL_Rect rect;
	SDL_GetWindowSize(window, &rect.w, &rect.h);
	rect.x = 0;
	rect.y = 0;
	SDL_RenderCopy(renderer, tex, NULL, &rect);
	SDL_DestroyTexture(tex);
}

void GraphicsManager::RenderExplosion(const Vector2& pos)
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);		//get background color
	
	SDL_Rect rect;	//set rect that represents drawing position for the explosion
	int IMAGE_SIZE = SetPictureSize();
	rect.x = pos.x * IMAGE_SIZE + GetWidthOffSet();
	rect.y = pos.y * IMAGE_SIZE + GetHeightOffset();
	rect.w = IMAGE_SIZE;
	rect.h = IMAGE_SIZE;

	SDL_Texture* backTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);	//create a texture that represents background
	if (backTexture == NULL)
	{
		printf("Error reating background texture : %s", SDL_GetError());
		exit(0);
	}
	SDL_SetTextureColorMod(backTexture, r, g, b);

	for (int i = 1; i <= 3; i++)
	{
		SDL_Texture* tex = SetTexture(SDL_GetBasePath(), "Explosion" + std::to_string(i));	//draw explosion
		SDL_RenderCopy(renderer, tex, NULL, &rect);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(tex);
		SDL_Delay(100);
		SDL_RenderCopy(renderer, backTexture, NULL, &rect);	//...then wait 100ms and overlay it with background
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(backTexture);
}

SDL_Renderer* GraphicsManager::GetRenderer()
{
	return renderer;
}