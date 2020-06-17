#include "CollectibleItem.hpp"

CollectibleItem::CollectibleItem(int x, int y, std::string name_) : Item(x, y, name_)
{
	SetPosition(x, y);
	name = name_;
	taken = false;
	graphics = GraphicsManager::Instance();
	IMAGE_SIZE = graphics->SetPictureSize();
	drawPosition.x = x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = y * IMAGE_SIZE + graphics->GetHeightOffset();
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	texture = graphics->SetTexture(name);
}

CollectibleItem::~CollectibleItem()
{
	texture = NULL;
}

Vector2& CollectibleItem::GetPosition()
{
	return pos;
}

Vector2& CollectibleItem::GetDirection()
{
	return pos;
}

Vector2& CollectibleItem::GetPreviousPosition()
{
	return pos;
}

void CollectibleItem::SetPosition(const int x, const int y)
{
	pos.x = x;
	pos.y = y;
}

std::string& CollectibleItem::GetName()
{
	return name;
}

bool CollectibleItem::Collectible()
{
	return true;
}

void CollectibleItem::Render()
{
	//check if item isn't already taken
	//if it is already taken, it now represents some train wagon, so it musn't be rendered
	if(!taken)
		SDL_RenderCopy(graphics->GetRenderer(), texture, NULL, &drawPosition);
}