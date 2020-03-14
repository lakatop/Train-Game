#include "InputManager.h"

InputManager* InputManager::instance = NULL;

InputManager* InputManager::Instance()
{
	if (instance == NULL)
		instance = new InputManager();
	return instance;
}

InputManager::InputManager()
{
	Update();
}

InputManager::~InputManager()
{
}

void InputManager::Clear()
{
	delete instance;
	instance = NULL;
}

bool InputManager::KeyDown(SDL_Scancode scanCode)
{
	//returns 1 if concrete key has been pressed, otherwise returns 0
	return keyboardStates[scanCode];
}

void InputManager::Update()
{
	//update pressed keys
	keyboardStates = SDL_GetKeyboardState(NULL);
}

