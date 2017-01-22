
#include "SFXManager.h"

#include <SpehsEngine/SoundSource.h>
#include <SpehsEngine/AudioManager.h>

#include <algorithm>


extern const float SOUNDFACTOR;


SFXManager::SFXManager()
{
}
SFXManager::~SFXManager()
{
	for (unsigned i = 0; i < activeSounds.size(); i++)
	{
		delete activeSounds[i];
	}
	for (unsigned i = 0; i < freeSounds.size(); i++)
	{
		delete freeSounds[i];
	}
}


void SFXManager::update()
{
	for (unsigned i = 0; i < activeSounds.size(); i++)
	{
		if (!activeSounds[i]->isPlaying())
		{
			freeSounds.push_back(activeSounds[i]);
			activeSounds[i] = activeSounds.back();
			activeSounds.pop_back();
			i--;
		}
	}
}
void SFXManager::playSound(const std::string& _file, const glm::vec2& _position, const float distance, const float _pitch, const float _gain)
{
	if (freeSounds.size())
	{
		activeSounds.push_back(freeSounds.back());
		freeSounds.pop_back();
	}
	else
	{
		activeSounds.push_back(new spehs::audio::ActiveSoundSource);
	}
	activeSounds.back()->setSound(spehs::AudioManager::instance->loadWAVE(_file));
	activeSounds.back()->setPosition(glm::vec2(_position.x / SOUNDFACTOR, _position.y / SOUNDFACTOR));
	activeSounds.back()->setZ(distance * 3.0f);
	activeSounds.back()->setPitch(_pitch);
	activeSounds.back()->setGain(_gain);
	activeSounds.back()->play();
}
