#include "CollectibleItem.h"

CollectibleItem::CollectibleItem(int x, int y, std::string name_) : Item(x, y, name_)
{
	IMAGE_SIZE = 50;
	pos.x = x;
	pos.y = y;
	name = name_;
	taken = false;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	graphics = GraphicsManager::Instance();
	texture = SetTexture(SDL_GetBasePath(), name);
}

CollectibleItem::~CollectibleItem()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
}

Vector2& CollectibleItem::GetPosition()
{
	return Item::GetPosition();
}

Vector2& CollectibleItem::GetDirection()
{
	return pos;
}

Vector2& CollectibleItem::GetPreviousPosition()
{
	return pos;
}

void CollectibleItem::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

std::string& CollectibleItem::GetName()
{
	return name;
}

SDL_Texture* CollectibleItem::SetTexture(std::string path, std::string name)
{
	return Item::SetTexture(path,name);
}

bool CollectibleItem::Collectible()
{
	return true;
}

void CollectibleItem::Render()
{
	if(!taken)
		SDL_RenderCopy(graphics->GetRenderer(), texture, NULL, &drawPosition);
}