/*
Peter Lakatos
AudioManager.h
Train_Game
*/
#ifndef _AUDIOMANAGER_H
#define _AUDIOMANAGER_H

#include <SDL_mixer.h>
#include <SDL.h>
#include <map>
#include <string>

class AudioManager
{
public:
	static AudioManager* Instance();
	void Clear();
	void PlayEffect(std::string name);
	void PlayMusic(std::string name);
	void StopMusic();

private:
	AudioManager();
	~AudioManager();
	Mix_Chunk* GetEffect(std::string name);
	Mix_Music* GetMusic(std::string name);

	static AudioManager* instance;
	
	std::map<std::string, Mix_Chunk*> soundEffects;
	std::map<std::string, Mix_Music*> soundMusic;
};

#endif