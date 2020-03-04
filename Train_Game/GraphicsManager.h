#ifndef _GRAPHICSMANAGER_H
#define _GRAPHICSMANAGER_H
#include<SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

class GraphicsManager
{
public:
	static GraphicsManager* Instance();
	bool ReturnSucces();
	void Render(std::string path, int height);
	void Clear();
	SDL_Renderer* GetRenderer();
	double GetFlipAngle(int x, int y);

	SDL_Window* window;
	SDL_Surface* back_buffer;
	SDL_Renderer* renderer;
	SDL_RendererFlip flip;
	SDL_Rect renderRect;

private:
	GraphicsManager();
	~GraphicsManager();
	bool Init();
	void SetFlipAngleArray();
	SDL_Texture* CreateText(std::string path);
	SDL_Texture* GetText(std::string path);

	static GraphicsManager* instance;
	std::map<std::string, SDL_Texture*> text;
	bool success_initialization;
	double flipArray[3][3];
	int IMAGE_SIZE;
	int FONT_SIZE;
	int SCREEN_WIDTH;
	int SCREEN_HEIGT;
};

#endif
