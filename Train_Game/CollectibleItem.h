#ifndef _COLLECTIBLEITEM_H
#define _COLLECTIBLEITEM_H

#include "Item.h"

class CollectibleItem : public Item
{
public:
	CollectibleItem(int x, int y, std::string name_);
	~CollectibleItem();
	Vector2& GetPosition() override;
	std::string& GetName() override;
	void Render();
	bool Collectible();
	Vector2& GetDirection();
	Vector2& GetPreviousPosition();
	void Update() {}

	bool taken;
private:
	void SetPosition(int x, int y) override;
	SDL_Texture* SetTexture(std::string path, std::string name) override;

	int IMAGE_SIZE;
	std::string name;
	Vector2 pos;
	SDL_Rect drawPosition;
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif