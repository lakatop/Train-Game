#include "Item.h"

Item::Item()
{
	graphics = NULL;
	texture = NULL;
}

Item::Item(int x, int y, std::string name_)
{
	pos.x = x;
	pos.y = y;
	name = name_;
}

Vector2& Item::GetPosition()
{
	return pos;
}

void Item::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

SDL_Texture* Item::SetTexture(std::string path)
{
	return SetTexture(path, name);
}

SDL_Texture* Item::SetTexture(std::string path, std::string name)
{
	SDL_Texture* tex = NULL;
	std::string fullPath = path.append("Art/") + name + ".png";
	SDL_Surface* surface = IMG_Load(fullPath.c_str());
	if (surface == NULL)
	{
		printf("Error loading image : %s\n", IMG_GetError());
		exit(0);
	}
	tex = SDL_CreateTextureFromSurface(graphics->GetRenderer(), surface);
	if (tex == NULL)
	{
		printf("Error creating texture : %s\n", SDL_GetError());
		exit(0);
	}
	SDL_FreeSurface(surface);
	return tex;
}