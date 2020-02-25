#include "TrainComponent.h"

TrainComponent::TrainComponent(int x, int y, std::string name_, Component* par)
{
	pos.x = x;
	pos.y = y;
	IMAGE_SIZE = 50;
	parent = par;
	name = name_;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	graphics = GraphicsManager::Instance();
	texture = SetTexture(SDL_GetBasePath());
}

TrainComponent::~TrainComponent()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
	delete parent;
	parent = NULL;
}

Vector2& TrainComponent::GetPosition()
{
	return pos;
}

void TrainComponent::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void TrainComponent::UpdatePosition()
{
	drawPosition.x = pos.x;
	drawPosition.y = pos.y;
}

SDL_Texture* TrainComponent::SetTexture(std::string path)
{
	SDL_Texture* tex = NULL;
	std::string fullPath = path.append("Art/") + name + ".png";
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

void TrainComponent::SetMoveDirection()
{
	moveDirection.x = GetParentDirection().x;
	moveDirection.y = GetParentDirection().y;
}

void TrainComponent::Update()
{
	SetMoveDirection();
	pos.x += moveDirection.x;
	pos.y += moveDirection.y;
	drawPosition.x = pos.x * IMAGE_SIZE;
	drawPosition.y = pos.y * IMAGE_SIZE;
}

Vector2& TrainComponent::GetParentDirection()
{
	return parent->GetDirection();
}

Vector2& TrainComponent::GetDirection()
{
	return moveDirection;
}

void TrainComponent::Render()
{
	SDL_RenderCopy(graphics->GetRenderer(), texture, NULL, &drawPosition);
}