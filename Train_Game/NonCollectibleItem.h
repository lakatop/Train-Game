/*
Peter Lakatos
NonCollectibleItem.h
Train_Game
*/
#ifndef _NONCOLLECTIBLEITEM_H
#define _NONCOLLECTIBLEITEM_H

#include "Item.h"

class NonCollectibleItem : public Item
{
public:
	NonCollectibleItem(int x, int y, std::string name_, bool brick_ = false, Vector2 dir = (0, 0));
	~NonCollectibleItem();
	Vector2& GetPosition() override;
	std::string& GetName() override;
	void Render();
	bool Collectible();
	Vector2& GetDirection();
	Vector2& GetPreviousPosition();
	void Update() {}
	bool GetFire() { return false; }
	void SetFire() {}

	bool brick;

private:
	void SetPosition(const int x, const int y) override;

	GraphicsManager* graphics;

	int IMAGE_SIZE;
	std::string name;
	
	Vector2 pos;
	Vector2 direction;
	
	SDL_Rect drawPosition;
	SDL_Texture* texture;
};

#endif
