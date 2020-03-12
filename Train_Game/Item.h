#ifndef _ITEM_H
#define _ITEM_H

/*
Peter Lakatos
Item.h
Train_Game
*/
#include "Component.h"
#include "GraphicsManager.h"

class Item : public Component
{
public:
	Item(int x, int y, std::string& name_, bool brick_ = false);
	Item();
	virtual ~Item();
	Vector2& GetPosition() override;
	virtual std::string& GetName() = 0;
	virtual Vector2& GetPreviousPosition() = 0;
	virtual void Render() = 0;
	virtual Vector2& GetDirection() = 0;
	virtual void Update() = 0;
	virtual bool Collectible() = 0;
	virtual bool GetFire() = 0;
	virtual void SetFire() = 0;

	bool brick;

private:
	virtual void SetPosition(const int x, const int y) override;

	std::string name;
	Vector2 pos;
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif