#include"GameManager.hpp"

int main(int argc, char* argv[])
{
	GameManager* g = GameManager::Instance();
	g->GameLoop();
	g->Clear();
	return 0;
}