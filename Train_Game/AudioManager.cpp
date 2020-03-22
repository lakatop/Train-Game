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
	//initialize the mixer API and check if it was successful
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
	//free all sound effects
	for (auto&& [name, effect] : soundEffects)
		if (effect != NULL)
		{
			Mix_FreeChunk(effect);
			effect = NULL;
		}
	soundEffects.clear();

	//free all music
	for(auto&& [name, music] : soundMusic)
		if (music != NULL)
		{
			Mix_FreeMusic(music);
			music = NULL;
		}
	soundMusic.clear();

	delete instance;
	instance = NULL;

	//shutdown and cleanup the mixer API
	Mix_CloseAudio();
}

Mix_Chunk* AudioManager::GetEffect(std::string name)
{
	//find fullpath to specific file
	std::string path = SDL_GetBasePath();
	path.append("/Art/" + name + ".wav");

	//check if it is already in map
	//if not, add it
	auto it = soundEffects.find(name);
	if (it == soundEffects.end())
		soundEffects.insert({ name, Mix_LoadWAV(path.c_str()) });
	
	if (soundEffects[name] == nullptr)	//check if it was created successfully
	{
		printf("effect loading error : %s", Mix_GetError());
		exit(0);
	}

	return soundEffects[name];
}

Mix_Music* AudioManager::GetMusic(std::string name)
{
	//find fullpath to specific file
	std::string path = SDL_GetBasePath();
	path.append("/Art/" + name + ".mp3");

	//check if it is already in map
	//if not, add it
	auto it = soundMusic.find(name);
	if (it == soundMusic.end())
		soundMusic.insert({ name, Mix_LoadMUS(path.c_str()) });
	
	if (soundMusic[name] == nullptr)	//check if it was created successfully
	{
		printf("music loading error : %s", Mix_GetError());
		exit(0);
	}

	return soundMusic[name];
}

void AudioManager::PlayEffect(std::string name)
{
	//play chunk on channel 1 with no loops
	Mix_PlayChannel(1, GetEffect(name), 0);
	//set lower volume
	Mix_VolumeChunk(GetEffect(name), MIX_MAX_VOLUME / 25);
}

void AudioManager::PlayMusic(std::string name)
{
	//set lower volume
	Mix_VolumeMusic(MIX_MAX_VOLUME / 25);
	//play loaded music, -1 = play it forever
	Mix_PlayMusic(GetMusic(name), -1);
}

void AudioManager::StopMusic()
{
	//check if music is playing
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();	//pause currently playing music...
		Mix_RewindMusic();	//...and rewind it to the beginning
	}
}