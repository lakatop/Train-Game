#include "CollectibleItem.h"

CollectibleItem::CollectibleItem(int x, int y, std::string name_) : Item(x, y, name)
{
	IMAGE_SIZE = 50;
	pos.x = x;
	pos.y = y;
	name = name_;
	taken = false;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
}

Vector2& CollectibleItem::GetPosition()
{
	return Item::GetPosition();
}

void CollectibleItem::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

SDL_Texture* CollectibleItem::SetTexture(std::string path, std::string name)
{
	return Item::SetTexture(path,name);
}