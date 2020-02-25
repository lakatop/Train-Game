#include "Locomotive.h"

Locomotive* Locomotive::instance = NULL;

Locomotive* Locomotive::Instance(int x, int y)
{
	if (instance == NULL)
		instance = new Locomotive(x, y);
	return instance;
}

Vector2& Locomotive::GetDirection()
{
	return moveDirection;
}

Vector2& Locomotive::GetParentDirection()
{
	return moveDirection;
}

Locomotive::Locomotive(int x, int y)
{
	pos.x = x;
	pos.y = y;
	IMAGE_SIZE = 50;
	moving = false;
	parent = NULL;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	graphics = GraphicsManager::Instance();
	texture = SetTexture(SDL_GetBasePath());
}

Locomotive::~Locomotive()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
}

void Locomotive::SetMoveDirection(const Vector2& m)
{
	moveDirection.x = m.x;
	moveDirection.y = m.y;
}

void Locomotive::Update()
{
	pos.x += moveDirection.x;
	pos.y += moveDirection.y;
	drawPosition.x = pos.x * IMAGE_SIZE;
	drawPosition.y = pos.y * IMAGE_SIZE;
}

Vector2& Locomotive::GetPosition() 
{
	return pos;
}

void Locomotive::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void Locomotive::UpdatePosition()
{
	drawPosition.x = pos.x;
	drawPosition.y = pos.y;
}

SDL_Texture* Locomotive::SetTexture(std::string path)
{
	SDL_Texture* tex = NULL;
	std::string fullPath = path.append("Art/locomotive.png");
	SDL_Surface* surface = IMG_Load(fullPath.c_str());
	if (surface == NULL)
	{
		printf("Error loading image : %s\n", IMG_GetError());
		exit(0);
	}
	tex = SDL_CreateTextureFromSurface(graphics->GetRenderer(), surface);
	if (tex == NULL)
	{
		printf("Error creating texture : %s\n", SDL_GetError());
		exit(0);
	}
	SDL_FreeSurface(surface);
	return tex;
}

void Locomotive::Render()
{
	SDL_RenderCopy(graphics->GetRenderer(), texture, NULL, &drawPosition);
}