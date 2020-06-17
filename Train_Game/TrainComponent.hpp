/*
Peter Lakatos
TrainComponent.h
Train_Game
*/
#ifndef _TRAINCOMPONENT_H
#define _TRAINCOMPONENT_H

#include "Component.hpp"
#include "GraphicsManager.hpp"

class TrainComponent : public Component
{
public:
	TrainComponent(int x, int y, std::string& name_, Vector2& move_, bool last_ = false, Component* par = NULL);
	Vector2& GetPosition() override;
	std::string& GetName() override;
	Vector2& GetPreviousPosition() override;
	void Render() override;
	Vector2& GetDirection() override;
	void Update() override;
	bool GetFire() override;
	void SetFire() override;
	bool GetLast();
	~TrainComponent();
	void CheckFireCollision();
	Vector2& GetParentDirection();
	Vector2& GetPreviousMoveDirection();
	void SetMoveDirection();
	void SetPreviousMoveDirection();
	void SetToPreviousPosition();
	Component* GetPointer();

	Component* parent;

private:
	void SetPosition(const int x, const int y) override;

	GraphicsManager* graphics;

	bool last;
	bool fire;
	int IMAGE_SIZE;
	std::string name;
	
	Vector2 pos;
	Vector2 previousPos;
	Vector2 moveDirection;
	Vector2 previousMoveDirection;
	
	SDL_Rect drawPosition; //every component will have its own rect, which will represent its position on screen and will be updated every frame
	SDL_Texture* texture;
	SDL_Texture* fireTex;
};

#endif