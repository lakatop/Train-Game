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
	delete keyboardStates;
	keyboardStates = NULL;
}

void InputManager::Clear()
{
	delete instance;
	instance = NULL;
}

bool InputManager::KeyDown(SDL_Scancode scanCode)
{
	return keyboardStates[scanCode];
}

void InputManager::Update()
{
	keyboardStates = SDL_GetKeyboardState(NULL);
}

