/*
Peter Lakatos
Locomotive.h
Train_Game
*/
#ifndef _LOCOMOTIVE_H
#define _LOCOMOTIVE_H

#include "Component.hpp"
#include "GraphicsManager.hpp"

class Locomotive : public Component
{
public:
	static Locomotive* Instance(const int x, const int y);
	Vector2& GetPosition() override;
	void Render() override;
	void SetMoveDirection(const Vector2& m);
	void Update() override;
	Vector2& GetDirection() override;
	Vector2& GetPreviousPosition() override;
	std::string& GetName() override;
	bool GetFire() override;
	void SetFire() override;
	void SetToPreviousPosition();
	void Clear();

	bool moving;
	Component* parent;

private:
	void SetPosition(const int x, const int y);
	Locomotive(int x, int y);
	~Locomotive();

	GraphicsManager* graphics;

	bool fire;
	static Locomotive* instance;
	int IMAGE_SIZE;
	std::string name;
	
	Vector2 pos;
	Vector2 previousPos;
	Vector2 moveDirection;
	
	SDL_Rect drawPosition;
	SDL_Texture* texture;
};

#endif