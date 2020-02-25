#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "GraphicsManager.h"

struct Vector2
{
public:
	Vector2(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	int x;
	int y;
	Vector2& operator=(const Vector2& rhs) //not needed yet
	{
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}
private:
	bool operator==(const Vector2& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}
};

class Component
{
public:
	virtual Vector2& GetPosition() = 0;
	virtual void Render() = 0;
	virtual Vector2& GetParentDirection() = 0;
	virtual Vector2& GetDirection() = 0;
	virtual void Update() = 0;
private:
	virtual void SetPosition(int x, int y) = 0;
	virtual SDL_Texture* SetTexture(std::string path) = 0;
};

#endif