#include "LevelManager.hpp"

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
	//delete all levels
	for (auto&& x : levels)
		x = nullptr;
	
	delete instance;
	instance = NULL;
}

int LevelManager::GetLevelCount()
{
	//return number of levels
	return levelCount;
}

int LevelManager::GetLevelWidth(const int levelNumber)
{
	return levels[levelNumber - 1]->GetWidth();
}

int LevelManager::GetLevelHeight(const int levelNumber)
{
	return levels[levelNumber - 1]->GetHeight();
}

char LevelManager::GetLevelChar(const int x, const int y, const int levelNumber)
{
	//return a character on concrete position of a concrete level
	return levels[levelNumber - 1]->GetChar(levels[levelNumber - 1]->index(x, y));
}

void LevelManager::LoadLevels()
{
	//create path to the directory with levels
	std::string path = SDL_GetBasePath();
	path.append("Maps");
	
	//get number of levels
	for (auto const& file : std::filesystem::directory_iterator(path))
		levelCount++;
	if (levelCount == 0)
	{
		printf("Error : no available levels");
		exit(0);
	}
	
	//load all levels
	std::ifstream f;
	for (int i = 1; i <= levelCount; i++)
	{
		//create path to concrete level
		std::string mapName = path + "/map" + std::to_string(i) + ".txt";
		
		//open file of level
		f.open(mapName);
		if (!f.good())
		{
			printf("Error reading map file");
			exit(0);
		}
		
		//get width and height of level
		std::string line;
		std::getline(f, line);
		int width = std::stoi(line);
		std::getline(f, line);
		int height = std::stoi(line);
		
		//create level and add it to the vector of levels
		levels.emplace_back(std::make_unique<LevelMap>(width,height));
		
		//iterate through rows
		for (int r = 0; r < height; r++)
		{
			//iterate through collumns
			for (int c = 0; c < width+1; c++)
			{
				//get character on concrete position
				char x = f.get();
				if (f.fail())
				{
					printf("Error reading map file");
					exit(0);
				}
				//if character isn't newline, set it up
				if (x != '\n')
					levels[i - 1]->SetChar(x, r, c);
			}
		}
		f.close();
	}
}