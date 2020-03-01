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
	void Clear();
	SDL_Renderer* GetRenderer();
	double GetFlipAngle(int x, int y);

	SDL_Window* window;
	SDL_Surface* back_buffer;
	SDL_Renderer* renderer;
	SDL_RendererFlip flip;

private:
	GraphicsManager();
	~GraphicsManager();
	bool Init();
	void SetFlipAngleArray();

	static GraphicsManager* instance;
	bool success_initialization;
	double flipArray[3][3];
	int SCREEN_WIDTH;
	int SCREEN_HEIGT;
};

#endif
