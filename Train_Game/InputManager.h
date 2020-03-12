/*
Peter Lakatos
InputManager.h
Train_Game
*/
#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL.h>

class InputManager
{
public:
	static InputManager* Instance();
	void Clear();
	bool KeyDown(SDL_Scancode scanCode);
	void Update();

private:
	InputManager();
	~InputManager();

	static InputManager* instance;
	const Uint8* keyboardStates;
};

#endif