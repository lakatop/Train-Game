#ifndef _ITEM_H
#define _ITEM_H

#include "Component.h"

class Item : public Component
{
public:
	Item(int x, int y, std::string name_);
	Item();
	~Item();
	virtual Vector2& GetPosition() override;
	SDL_Texture* SetTexture(std::string path) override;
	virtual SDL_Texture* SetTexture(std::string path, std::string name);
	virtual void Render() = 0;
	virtual Vector2& GetParentDirection() = 0;
	virtual Vector2& GetDirection() = 0;
	virtual void Update() = 0;
private:
	virtual void SetPosition(int x, int y) override;


	std::string name;
	Vector2 pos;
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif