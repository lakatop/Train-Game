#ifndef _TRAINCOMPONENT_H
#define _TRAINCOMPONENT_H

#include "Component.h"

class TrainComponent : public Component
{
public:
	TrainComponent(int x, int y, bool loc, std::string name_, TrainComponent* par = NULL);
	Vector2& GetPosition() override;
	SDL_Texture* SetTexture(std::string path) override;
	~TrainComponent();
private:
	void SetPosition(int x, int y) override;
	void UpdatePosition();

	TrainComponent* parent;
	bool locomotive;
	std::string name;
	Vector2 pos;
	Vector2 moveDirection;
	SDL_Rect drawPosition; //every component will have its own rect, which will represent its position on screen and will be updated every frame
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif