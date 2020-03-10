#include "NonCollectibleItem.h"

NonCollectibleItem::NonCollectibleItem(int x, int y, std::string name_, bool brick_, Vector2 dir) : Item(x, y, name_, brick_)
{
	pos.x = x;
	pos.y = y;
	name = name_;
	brick = brick_;
	direction = dir;
	graphics = GraphicsManager::Instance();
	IMAGE_SIZE = graphics->SetPictureSize();
	drawPosition.x = x * IMAGE_SIZE + graphics->GetWidthOffSet();
	drawPosition.y = y * IMAGE_SIZE + graphics->GetHeightOffset();
	drawPosition.w = IMAGE_SIZE;
	drawPosition.h = IMAGE_SIZE;
	texture = graphics->SetTexture(SDL_GetBasePath(), name);
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