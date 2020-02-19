#ifndef _GRAPHICSMANAGER_H
#define _GRAPHICSMANAGER_H
#include<SDL.h>
#include <stdio.h>
#include <SDL_image.h>

class GraphicsManager
{
public:
	static GraphicsManager* Instance();
	bool ReturnSucces();
	void Render();

	SDL_Window* window;
	SDL_Surface* back_buffer;
	SDL_Renderer* renderer;

private:
	GraphicsManager();
	~GraphicsManager();
	void Clear();
	bool Init();

	static GraphicsManager* instance;
	bool success_initialization;
	int SCREEN_WIDTH;
	int SCREEN_HEIGT;
};

#endif
