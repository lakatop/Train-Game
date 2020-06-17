/*
Peter Lakatos
LevelManager.h
Train_Game
*/
#ifndef _LEVELMANAGER_H
#define _LEVELMANAGER_H

#include <memory>
#include <vector>
#include <SDL.h>
#include <string>
#include <fstream>
#include<iostream>
#include<filesystem>
#include "Component.hpp"

// representing 1 level (map of the level)
struct LevelMap
{
public:
	LevelMap(size_t w, size_t h) : width(w), height(h), arr(std::make_unique<char[]>(width* h)) {}
	int index(const int x, const int y)
	{
		return (x * width) + y;
	}
	char GetChar(const int x)
	{
		return arr[x];
	}
	void SetChar(const char x, const int r, const int c)
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
	int GetLevelWidth(const int levelNumber);
	int GetLevelHeight(const int levelNumber);
	char GetLevelChar(const int x, const int y, const int levelNumber);

	int actualLevel;

private:
	LevelManager();
	~LevelManager();

	static LevelManager* instance;

	std::vector<std::unique_ptr<LevelMap>> levels;	//vector that contains all levels

	int levelCount;
};

#endif
