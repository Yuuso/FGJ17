
#pragma once

#include <vector>
#include <string>

#include <glm/vec3.hpp>


namespace spehs{ namespace audio{ class ActiveSoundSource; } }
class SFXManager
{
public:
	SFXManager();
	~SFXManager();

	void update();
	void playSound(const std::string& _file, const glm::vec2& _position, const float distance, const float _pitch = 1.0f, const float _gain = 1.0f);

private:
	std::vector<spehs::audio::ActiveSoundSource*> activeSounds;
	std::vector<spehs::audio::ActiveSoundSource*> freeSounds;
};

