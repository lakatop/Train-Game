#ifndef _LOCOMOTIVE_H
#define _LOCOMOTIVE_H

#include "Component.h"

class Locomotive : public Component
{
public:
	static Locomotive* Instance(int x, int y);
	Vector2& GetPosition() override;
	void Render() override;
	void SetMoveDirection(const Vector2& m);
	void Update() override;
	Vector2& GetDirection() override;
	Vector2& GetPreviousPosition() override;

	bool moving;
	Component* parent;
private:
	void SetPosition(int x, int y);
	SDL_Texture* SetTexture(std::string path);
	void UpdatePosition();
	Locomotive(int x, int y);
	~Locomotive();

	static Locomotive* instance;
	int IMAGE_SIZE;
	Vector2 pos;
	Vector2 previousPos;
	Vector2 moveDirection;
	SDL_Rect drawPosition;
	SDL_Texture* texture;
	GraphicsManager* graphics;
};

#endif