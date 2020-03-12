#include "Item.h"

Item::Item()
{
	graphics = NULL;
	texture = NULL;
	brick = true;
}

Item::Item(int x, int y, std::string& name_, bool brick_)
{
	SetPosition(x, y);
	name = name_;
	brick = brick_;
}

Item::~Item()
{
}

Vector2& Item::GetPosition()
{
	return pos;
}

void Item::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}