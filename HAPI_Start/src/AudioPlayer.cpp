#include "AudioPlayer.h"
#include "HAPI_lib.h"
#include "HAPI_Types.h"
#include "HAPI_InputCodes.h"
#include "Global.h"
#include <assert.h>

AudioPlayer::AudioClip::AudioClip(bool loop, float volume)
	: loop(loop),
	volume(0)
{
	if(volume < 0.0f)
	{ 
		volume = 0.0f;
	}
	else if (volume > 1.0f)
	{
		volume = 1.0f;
	}
}

//Commented out due to the fact that I didn't want to go about finding appropriate sounds for the game

bool AudioPlayer::loadAllSounds()
{
	//assert(m_audioClips.empty());
	//if (HAPI.LoadSound(DATA_DIRECTORY + "AUDIO_CLIP_NAME"))
	//{
	//	m_audioClips.emplace("AUDIO_CLIP_NAME", AudioClip(true, 1));
	//}
	//else
	//{
	//	return false;
	//}


	//HAPI.LoadSound(DATA_DIRECTORY + 'AUDIO CLIP NAME'

	return true;
}

void AudioPlayer::playSound(const std::string & audioClipName)
{
	auto iter = m_audioClips.find(audioClipName);
	assert(iter != m_audioClips.end());
	
	HAPISPACE::HAPI_TSoundOptions audioSettings(iter->second.volume, iter->second.loop, 0);
	bool soundPlayed = HAPI.PlaySound(audioClipName, audioSettings);
	assert(soundPlayed);
}