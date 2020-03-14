#include "TrainComponent.h"

TrainComponent::TrainComponent(int x, int y, std::string& name_, Vector2& move_, bool last_, Component* par)
{
	graphics = GraphicsManager::Instance();
	
	SetPosition(x, y);
	previousPos.x = x;
	previousPos.y = y;
	last = last_;
	parent = par;
	name = name_;
	moveDirection = move_;
	fire = false;
	
	IMAGE_SIZE = graphics->SetPictureSize();
	drawPosition.x = x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = y * IMAGE_SIZE + graphics->GetHeightOffset();
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	
	texture = graphics->SetTexture(name + "_wagon");
	fireTex = graphics->SetTexture("fire");
}

TrainComponent::~TrainComponent()
{
	texture = NULL;
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

	/*since Update() call is iterating through wagons from first to last,
	  i need to set current position to parent's previous position,
	  because parent's position has already been updated
	*/
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
	moveDirection = previousMoveDirection;
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
	//render train wagon
	SDL_RenderCopyEx(graphics->GetRenderer(), texture, NULL, &drawPosition, 
		graphics->GetFlipAngle(moveDirection.x, moveDirection.y), NULL, graphics->flip);
	
	//render fire
	if(fire)
		SDL_RenderCopyEx(graphics->GetRenderer(), fireTex, NULL, &drawPosition,
			graphics->GetFlipAngle(moveDirection.x, moveDirection.y), NULL, graphics->flip);
}