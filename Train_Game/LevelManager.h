#ifndef _LEVELMANAGER_H
#define _LEVELMANAGER_H

#include <memory>
#include <vector>
#include <SDL.h>
#include <string>
#include <fstream>
#include<iostream>
#include<filesystem>
#include "Component.h"

struct LevelMap	// representing 1 level (map of the level)
{
public:
	LevelMap(size_t w, size_t h) : width(w), height(h), arr(std::make_unique<char[]>(width* h)) {}
	int index(int x, int y)
	{
		return (x * width) + y;
	}
	char GetChar(int x)
	{
		return arr[x];
	}
	void SetChar(char x, int r, int c)
	{
		arr[r * width + c] = x;
	}
	int GetWidth()
	{
		return width;
	}
	int GetHeight()
	{
		return height;
	}
private:
	int height;
	int width;
	std::unique_ptr<char[]> arr;
};

class LevelManager
{
public:
	static LevelManager* Instance();
	void LoadLevels();
	void Clear();
	int GetLevelCount();
	int GetLevelWidth(int levelNumber);
	int GetLevelHeight(int levelNumber);
	char GetLevelChar(int x, int y, int levelNumber);

	int actualLevel;
private:
	LevelManager();
	~LevelManager();

	static LevelManager* instance;
	std::vector<std::unique_ptr<LevelMap>> levels;	//vector of levels
	int levelCount;
};

#endif
