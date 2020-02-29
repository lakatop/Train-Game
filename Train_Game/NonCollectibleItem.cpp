#include "NonCollectibleItem.h"

NonCollectibleItem::NonCollectibleItem(int x, int y, std::string name_, bool brick_, Vector2 dir) : Item(x, y, name_, brick_)
{
	IMAGE_SIZE = 50;
	pos.x = x;
	pos.y = y;
	name = name_;
	brick = brick_;
	direction = dir;
	drawPosition.x = x * IMAGE_SIZE;
	drawPosition.y = y * IMAGE_SIZE;
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	graphics = GraphicsManager::Instance();
	texture = SetTexture(SDL_GetBasePath(), name);
}

NonCollectibleItem::~NonCollectibleItem()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
}

Vector2& NonCollectibleItem::GetPosition()
{
	return Item::GetPosition();
}

void NonCollectibleItem::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

SDL_Texture* NonCollectibleItem::SetTexture(std::string path, std::string name)
{
	return Item::SetTexture(path, name);
}

bool NonCollectibleItem::Collectible()
{
	return false;
}

Vector2& NonCollectibleItem::GetDirection()
{
	return direction;
}

Vector2& NonCollectibleItem::GetPreviousPosition()
{
	return pos;
}

std::string& NonCollectibleItem::GetName()
{
	return name;
}

void NonCollectibleItem::Render()
{
	SDL_RenderCopy(graphics->GetRenderer(), texture, NULL, &drawPosition);
}