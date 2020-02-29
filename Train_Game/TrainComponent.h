#ifndef _TRAINCOMPONENT_H
#define _TRAINCOMPONENT_H

#include "Component.h"

class TrainComponent : public Component
{
public:
	TrainComponent(int x, int y, std::string name_, Component* par = NULL);
	Vector2& GetPosition() override;
	SDL_Texture* SetTexture(std::string path) override;
	void Render();
	~TrainComponent();
	void Update();
	Vector2& GetParentDirection();
	Vector2& GetDirection();
	Vector2& GetPreviousPosition();
	void SetMoveDirection();
	Component* GetPointer();

	Component* parent;
private:
	void SetPosition(int x, int y) override;
	void UpdatePosition();

	int IMAGE_SIZE;
	std::string name;
	Vector2 pos;
	Vector2 previousPos;
	Vector2 moveDirection;
	SDL_Rect drawPosition; //every component will have its own rect, which will represent its position on screen and will be updated every frame
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif