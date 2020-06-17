/*
Peter Lakatos
CollectibleItem.h
Train_Game
*/
#ifndef _COLLECTIBLEITEM_H
#define _COLLECTIBLEITEM_H

#include "Item.hpp"

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
	bool GetFire() { return false; }
	void SetFire() {}

	bool taken;

private:
	void SetPosition(const int x, const int y) override;
	
	GraphicsManager* graphics;

	int IMAGE_SIZE;
	std::string name;
	SDL_Rect drawPosition;
	
	Vector2 pos;
	
	SDL_Texture* texture;
};

#endif