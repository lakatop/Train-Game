/*
Peter Lakatos
GraphicsManager.h
Train_Game
*/
#ifndef _GRAPHICSMANAGER_H
#define _GRAPHICSMANAGER_H

#include<SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <map>
#include <string>
#include "LevelManager.h"

class GraphicsManager
{
public:
	static GraphicsManager* Instance();
	bool ReturnSuccess();
	void RenderScore(const std::string name, const int height);
	void Clear();
	void RenderSpecialScreen(const std::string name);
	void RenderExplosion(const Vector2& pos);
	int SetPictureSize();
	int GetWidthOffSet();
	int GetHeightOffset();
	SDL_Renderer* GetRenderer();
	SDL_Texture* SetTexture(const std::string& name);
	double GetFlipAngle(const int x, const int y);

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
	SDL_Texture* CreateText(std::string name);
	SDL_Texture* GetText(std::string name);
	SDL_Texture* GetTexture(std::string name);

	static GraphicsManager* instance;
	
	std::map<std::string, SDL_Texture*> text;
	std::map<std::string, SDL_Texture*> textures;
	
	bool success_initialization;
	double flipArray[3][3];
	int FONT_SIZE;
	int SCREEN_WIDTH;
	int SCREEN_HEIGT;
};

#endif
