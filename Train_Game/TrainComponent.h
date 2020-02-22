#ifndef _TRAINCOMPONENT_H
#define _TRAINCOMPONENT_H

#include "Component.h"

class TrainComponent : public Component
{
public:
	TrainComponent(int x, int y, bool loc, TrainComponent* par = NULL);
	virtual Vector2& GetPosition() override;
private:
	virtual void SetPosition(int x, int y) override;
	void UpdatePosition();
	SDL_Texture* SetTexture(std::string path);	//??? implement texture manager ???

	TrainComponent* parent;
	bool locomotive;
	Vector2 pos;
	Vector2 moveDirection;
	SDL_Rect drawPosition; //every component will have its own rect, which will represent its position on screen and will be updated every frame
	SDL_Texture* texture;
};

#endif