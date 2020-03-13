#include "AudioManager.h"

AudioManager* AudioManager::instance = NULL;

AudioManager* AudioManager::Instance()
{
	if (instance == NULL)
		instance = new AudioManager;

	return instance;
}

AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("Audio initialization error : %s", Mix_GetError());
		exit(0);
	}
}

AudioManager::~AudioManager()
{
}

void AudioManager::Clear()
{
	for (auto&& x : soundEffects)
		if (x.second != NULL)
		{
			Mix_FreeChunk(x.second);
			x.second = NULL;
		}

	for(auto&& x : soundMusic)
		if (x.second != NULL)
		{
			Mix_FreeMusic(x.second);
			x.second = NULL;
		}

	delete instance;
	instance = NULL;

	Mix_CloseAudio();
}

Mix_Chunk* AudioManager::GetEffect(std::string name)
{
	std::string path = SDL_GetBasePath();
	path.append("/Art/" + name + ".wav");
	auto it = soundEffects.find(name);
	if (it == soundEffects.end())
		soundEffects.insert(std::make_pair(name, Mix_LoadWAV(path.c_str())));
	if (soundEffects[name] == nullptr)
	{
		printf("effect loading error : %s", Mix_GetError());
		exit(0);
	}

	return soundEffects[name];
}

Mix_Music* AudioManager::GetMusic(std::string name)
{
	std::string path = SDL_GetBasePath();
	path.append("/Art/" + name + ".mp3");
	auto it = soundMusic.find(name);
	if (it == soundMusic.end())
		soundMusic.insert(std::make_pair(name, Mix_LoadMUS(path.c_str())));
	if (soundMusic[name] == nullptr)
	{
		printf("music loading error : %s", Mix_GetError());
		exit(0);
	}

	return soundMusic[name];
}

void AudioManager::PlayEffect(std::string name)
{
	Mix_PlayChannel(1, GetEffect(name), 0);
	Mix_VolumeChunk(GetEffect(name), MIX_MAX_VOLUME / 25);
}

void AudioManager::PlayMusic(std::string name)
{
	Mix_VolumeMusic(MIX_MAX_VOLUME / 25);
	Mix_PlayMusic(GetMusic(name), -1);
}

void AudioManager::StopMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
		Mix_RewindMusic();
	}
}