#pragma once

#include <unordered_map>
#include <string>

//Alows audio clips to be played throughout the codebase
//Commented out due to the fact that I didn't want to go about finding appropriate sounds for the game

class AudioPlayer
{
	struct AudioClip
	{
		AudioClip(bool loop, float volume);

		bool loop;
		float volume;
	};

public:
	static AudioPlayer& getInstance()
	{
		static AudioPlayer instance;
		return instance;
	}
	
	bool loadAllSounds();
	void playSound(const std::string& audioClipName);

private:
	AudioPlayer();
	std::unordered_map<std::string, AudioClip> m_audioClips;
};