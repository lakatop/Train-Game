#include "GraphicsManager.hpp"

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
	flip = SDL_FLIP_NONE;

	SetFlipAngleArray();

	success_initialization = GraphicsManager::Init();
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Clear()
{
	//destroy all score texts
	for (auto&& [name, text] : text)
		if (text != NULL)
		{
			SDL_DestroyTexture(text);
			text = NULL;
		}
	text.clear();

	//destroy all item, locomotive, train wagon and screen textures
	for(auto&& [name, texture] : textures)
		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
		}
	textures.clear();

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	delete instance;
	instance = NULL;
	success_initialization = false;
}

bool GraphicsManager::Init()
{
	//initialize SDL library
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL initialization error : %s", SDL_GetError());
		return false;
	}

	//create window
	window = SDL_CreateWindow("Train Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL window creation error : %s", SDL_GetError());
		return false;
	}
	
	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("SDL renderer creation error : %s", SDL_GetError());
		return false;
	}
	//set renderer drawcolor to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	//image and fonts initialization
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

SDL_Texture* GraphicsManager::SetTexture(const std::string& name)
{
	//check if it is already in map
	//if not, add it
	auto it = textures.find(name);
	if (it == textures.end())
	{
		textures.insert({ name, GetTexture(name) });
		if (textures[name] == NULL)	//check if it was created successfully
		{
			printf("Texture creating error : %s", SDL_GetError());
			exit(0);
		}
	}

	return textures[name];
}

SDL_Texture* GraphicsManager::GetTexture(std::string name)
{
	SDL_Texture* tex = NULL;
	
	//find fullpath to specific file
	std::string path = SDL_GetBasePath();
	std::string fullPath = path + "Art/" + name + ".png";
	
	//load surface
	SDL_Surface* surface = IMG_Load(fullPath.c_str());
	if (surface == NULL)	//check if load was successful
	{
		printf("Error loading image : %s\n", IMG_GetError());
		exit(0);
	}
	
	//create texture from loaded surface
	tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == NULL)	//check if creating was successful
	{
		printf("Error creating texture : %s\n", SDL_GetError());
		exit(0);
	}
	
	//free loaded surface
	SDL_FreeSurface(surface);
	
	return tex;
}

bool GraphicsManager::ReturnSuccess()
{
	return success_initialization;
}

int GraphicsManager::SetPictureSize()
{
	int screenWidth = 0;
	int screenHeight = 0;
	
	//get width and height of window
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	
	//divide the screen width(height) by the width(height) of actual level
	int sizeWidth = screenWidth / LevelManager::Instance()->GetLevelWidth(LevelManager::Instance()->actualLevel);
	int sizeHeight = screenHeight / LevelManager::Instance()->GetLevelHeight(LevelManager::Instance()->actualLevel);
	
	//return the minimum of those
	return std::min(sizeWidth, sizeHeight);
}

int GraphicsManager::GetHeightOffset()
{
	//get height of current level in pixels
	int levelHeight = SetPictureSize() * LevelManager::Instance()->GetLevelHeight(LevelManager::Instance()->actualLevel);
	
	//get height of window in pixels
	int screenHeight = 0;
	SDL_GetWindowSize(window, NULL, &screenHeight);
	
	//return centered size in pixels
	return (screenHeight - levelHeight) / 2;
}

int GraphicsManager::GetWidthOffSet()
{
	//get width of current level in pixels
	int levelWidth = SetPictureSize() * LevelManager::Instance()->GetLevelWidth(LevelManager::Instance()->actualLevel);
	
	//get width of window in pixels
	int screenWidth = 0;
	SDL_GetWindowSize(window, &screenWidth, NULL);
	
	//return centered size in pixels
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
	and that will be an index in the flipArray that holds which angle to pass
	*/
	flipArray[0][1] = 0;
	flipArray[2][1] = 0;
	flipArray[1][2] = 90;
	flipArray[1][0] = -90;
}

SDL_Texture* GraphicsManager::GetText(const std::string name)
{
	auto it = text.find(name);
	
	//check if it is already in map
	//if not, add it
	if (it == text.end())
	{
		text.insert({ name, CreateText(name) });
		if (text[name] == NULL)	//check if it was created successfully
			exit(0);
	}
	
	//set width and height of new texture to renderRect for later rendering
	SDL_QueryTexture(text.find(name)->second, NULL, NULL, &renderRect.w, &renderRect.h);

	return text[name];
}

SDL_Texture* GraphicsManager::CreateText(const std::string name)
{
	//create fullpath to Retro Gaming font
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("/Art/Retro Gaming.ttf");
	
	//initialize font
	TTF_Font* font = TTF_OpenFont(fullPath.c_str(),FONT_SIZE);
	if (font == NULL)	//check if initialization was successful
	{
		printf("Font creating error : %s", TTF_GetError());
		return NULL;
	}
	
	//create surface with concrete font and text
	SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), { 255,255,255 });
	if (surface == NULL)	//check if creation was successful
	{
		printf("Text render error : %s", TTF_GetError());
		return NULL;
	}

	//create texture from loaded surface
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
	if (tex == NULL)	//check if creation was successful
	{
		printf("Text texture creating error : %s", SDL_GetError());
		return NULL;
	}

	//free loaded surface
	SDL_FreeSurface(surface);
	
	return tex;
}

void GraphicsManager::RenderScore(const std::string name, const int height)
{
	//render SCORE
	//renderRect.y = height of level in pixels + font size -> it will be rendered just under the last item ont the screen
	renderRect.y = height + FONT_SIZE;
	SDL_RenderCopy(renderer, GetText(name), NULL, &renderRect);
}

void GraphicsManager::RenderSpecialScreen(std::string name)
{
	//get concrete texture
	SDL_Texture* tex = SetTexture(name);
	
	//set its rendering size -> it will cover the whole window screen
	SDL_Rect rect;
	SDL_GetWindowSize(window, &rect.w, &rect.h);
	rect.x = 0;
	rect.y = 0;
	
	SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void GraphicsManager::RenderExplosion(const Vector2& pos)
{
	//get background color
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	
	//set rect that represents drawing position for the explosion
	SDL_Rect rect;
	int IMAGE_SIZE = SetPictureSize();
	rect.x = pos.x * IMAGE_SIZE + GetWidthOffSet();
	rect.y = pos.y * IMAGE_SIZE + GetHeightOffset();
	rect.w = IMAGE_SIZE;
	rect.h = IMAGE_SIZE;

	//create a texture that represents background
	SDL_Texture* backTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);
	if (backTexture == NULL)	//check if creation was successful
	{
		printf("Error reating background texture : %s", SDL_GetError());
		exit(0);
	}
	//set texture color to background color
	SDL_SetTextureColorMod(backTexture, r, g, b);

	//render all 3 phases of explosion
	for (int i = 1; i <= 3; i++)
	{
		//draw explosion
		SDL_Texture* tex = SetTexture("Explosion" + std::to_string(i));
		SDL_RenderCopy(renderer, tex, NULL, &rect);
		SDL_RenderPresent(renderer);

		//wait 100ms and overlay it with background
		SDL_Delay(100);
		SDL_RenderCopy(renderer, backTexture, NULL, &rect);	
		SDL_RenderPresent(renderer);
	}
	
	//destroy texture that represents background
	SDL_DestroyTexture(backTexture);
}

SDL_Renderer* GraphicsManager::GetRenderer()
{
	return renderer;
}