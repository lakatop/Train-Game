#include "TrainComponent.h"

TrainComponent::TrainComponent(int x, int y, bool loc, TrainComponent* par)
{
	pos.x = x;
	pos.y = y;
	locomotive = loc;
	parent = par;
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