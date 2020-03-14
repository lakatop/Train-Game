#include "Locomotive.h"

Locomotive* Locomotive::instance = NULL;

Locomotive* Locomotive::Instance(const int x, const int y)
{
	if (instance == NULL)
		instance = new Locomotive(x, y);
	return instance;
}

Locomotive::Locomotive(int x, int y)
{
	graphics = GraphicsManager::Instance();
	
	SetPosition(x, y);
	
	previousPos.x = x;
	previousPos.y = y;
	moving = false;
	parent = NULL;
	fire = false;
	name = "locomotive";
	moveDirection.x = 1;
	moveDirection.y = 0;
	
	IMAGE_SIZE = graphics->SetPictureSize();
	drawPosition.x = x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = y * IMAGE_SIZE + graphics->GetHeightOffset();
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	
	texture = graphics->SetTexture(name);
}

Vector2& Locomotive::GetDirection()
{
	return moveDirection;
}

void Locomotive::Clear()
{
	delete instance;
	instance = NULL;
}

Locomotive::~Locomotive()
{
	texture = NULL;
}

void Locomotive::SetMoveDirection(const Vector2& m)
{
	moveDirection.x = m.x;
	moveDirection.y = m.y;
}

void Locomotive::Update()
{
	previousPos.x = pos.x;
	previousPos.y = pos.y;
	pos.x += moveDirection.x;
	pos.y += moveDirection.y;
	drawPosition.x = pos.x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = pos.y * IMAGE_SIZE + graphics->GetHeightOffset();
}

std::string& Locomotive::GetName()
{
	return name;
}

bool Locomotive::GetFire()
{
	return fire;
}

void Locomotive::SetFire()
{
	fire = true;
}

Vector2& Locomotive::GetPreviousPosition()
{
	return previousPos;
}

Vector2& Locomotive::GetPosition() 
{
	return pos;
}

void Locomotive::SetPosition(const int x, const int y)
{
	pos.x = x;
	pos.y = y;
}

void Locomotive::SetToPreviousPosition()
{
	pos = previousPos;
	drawPosition.x = pos.x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = pos.y * IMAGE_SIZE + graphics->GetHeightOffset();
}

void Locomotive::Render()
{
	//get flip angle
	double flipAngle = graphics->GetFlipAngle(moveDirection.x, moveDirection.y);
	//render locomotive
	SDL_RenderCopyEx(graphics->GetRenderer(), texture, NULL, &drawPosition, flipAngle, NULL, graphics->flip);
}