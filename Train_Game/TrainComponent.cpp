#include "TrainComponent.h"

TrainComponent::TrainComponent(int x, int y, std::string name_, Vector2 move_, bool last_, Component* par)
{
	pos.x = x;
	pos.y = y;
	previousPos.x = x;
	previousPos.y = y;
	IMAGE_SIZE = 50;
	last = last_;
	parent = par;
	name = name_;
	moveDirection = move_;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	fire = false;
	graphics = GraphicsManager::Instance();
	std::string basePath = SDL_GetBasePath();
	texture = SetTexture(basePath + "Art/" + name + "_wagon.png");
	fireTex = SetTexture(basePath + "Art/fire.png");
}

TrainComponent::~TrainComponent()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
	SDL_DestroyTexture(fireTex);
	fireTex = NULL;
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
	SDL_Surface* surface = IMG_Load(path.c_str());
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
	previousMoveDirection = moveDirection;
	moveDirection.x = GetParentDirection().x;
	moveDirection.y = GetParentDirection().y;
}

void TrainComponent::SetPreviousMoveDirection()
{
	moveDirection = previousMoveDirection;
}

Vector2& TrainComponent::GetPreviousMoveDirection()
{
	return previousMoveDirection;
}

void TrainComponent::Update()
{
	previousPos.x = pos.x;
	previousPos.y = pos.y;
	pos.x = parent->GetPreviousPosition().x;
	pos.y = parent->GetPreviousPosition().y;
	drawPosition.x = pos.x * IMAGE_SIZE;
	drawPosition.y = pos.y * IMAGE_SIZE;
}

void TrainComponent::CheckFireCollision()
{
	//check whether there is a tree and a lighter wagon next to each other
	if ((this->GetName() == "tree" && parent->GetName() == "lighter")
		|| (this->GetName() == "lighter" && parent->GetName() == "tree"))
	{
		this->fire = true;
	}
}

std::string& TrainComponent::GetName()
{
	return name;
}

bool TrainComponent::GetFire()
{
	return fire;
}

void TrainComponent::SetFire()
{
	fire = true;
}

bool TrainComponent::GetLast()
{
	return last;
}

Vector2& TrainComponent::GetPreviousPosition()
{
	return previousPos;
}

Component* TrainComponent::GetPointer()
{
	return this;
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
	SDL_RenderCopyEx(graphics->GetRenderer(), texture, NULL, &drawPosition, 
		graphics->GetFlipAngle(moveDirection.x, moveDirection.y), NULL, graphics->flip);
	if(fire)
		SDL_RenderCopyEx(graphics->GetRenderer(), fireTex, NULL, &drawPosition,
			graphics->GetFlipAngle(moveDirection.x, moveDirection.y), NULL, graphics->flip);
}