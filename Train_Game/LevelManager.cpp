#include "LevelManager.h"

LevelManager* LevelManager::instance = NULL;

LevelManager* LevelManager::Instance()
{
	if (instance == NULL)
		instance = new LevelManager();
	return instance;
}

LevelManager::LevelManager()
{
	actualLevel = 0;
	levelCount = 0;
	LoadLevels();
}

LevelManager::~LevelManager()
{
}

void LevelManager::Clear()
{
	for (auto&& x : levels)
		x = nullptr;
	delete instance;
}

int LevelManager::GetActualLevel()
{
	return ++actualLevel;
}

int LevelManager::GetLevelCount()
{
	return levelCount;
}

int LevelManager::GetLevelWidth(int levelNumber)
{
	return levels[levelNumber - 1]->GetWidth();
}

int LevelManager::GetLevelHeight(int levelNumber)
{
	return levels[levelNumber - 1]->GetHeight();
}

char LevelManager::GetLevelChar(int x, int y, int levelNumber)
{
	return levels[levelNumber - 1]->GetChar(levels[levelNumber - 1]->index(x, y));
}

void LevelManager::LoadLevels()
{
	std::string path = SDL_GetBasePath();
	path.append("Maps");
	for (auto const& file : std::filesystem::directory_iterator(path))
		levelCount++;
	if (levelCount == 0)
	{
		printf("Error : no available levels");
		exit(0);
	}
	std::ifstream f;
	for (int i = 1; i <= levelCount; i++)
	{
		std::string mapName = path.append("/map") + std::to_string(i) + ".txt";
		f.open(mapName);
		if (!f.good())
		{
			printf("Error reading map file");
			exit(0);
		}
		std::string line;
		std::getline(f, line);
		int width = std::stoi(line);
		std::getline(f, line);
		int height = std::stoi(line);
		levels.emplace_back(std::make_unique<LevelMap>(width,height));
		for (int r = 0; r < height; r++)	//iterate through rows
		{
			for (int c = 0; c < width+1; c++)		//iterate through collumns
			{
				char x = f.get();
				if (f.fail())
				{
					printf("Error reading map file");
					exit(0);
				}
				if (x != '\n')
					levels[i - 1]->SetChar(x, r, c);
			}
		}
		f.close();
	}
}