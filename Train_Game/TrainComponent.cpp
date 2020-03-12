#include "TrainComponent.h"

TrainComponent::TrainComponent(int x, int y, std::string& name_, Vector2& move_, bool last_, Component* par)
{
	SetPosition(x, y);
	previousPos.x = x;
	previousPos.y = y;
	last = last_;
	parent = par;
	name = name_;
	moveDirection = move_;
	fire = false;
	graphics = GraphicsManager::Instance();
	IMAGE_SIZE = graphics->SetPictureSize();
	drawPosition.x = x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = y * IMAGE_SIZE + graphics->GetHeightOffset();
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	std::string basePath = SDL_GetBasePath();
	texture = graphics->SetTexture(basePath,name + "_wagon");
	fireTex = graphics->SetTexture(basePath,"fire");
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

void TrainComponent::SetPosition(const int x, const int y)
{
	pos.x = x;
	pos.y = y;
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
	drawPosition.x = pos.x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = pos.y * IMAGE_SIZE + graphics->GetHeightOffset();
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

void TrainComponent::SetToPreviousPosition()
{
	pos = previousPos;
	drawPosition.x = pos.x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = pos.y * IMAGE_SIZE + graphics->GetHeightOffset();
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