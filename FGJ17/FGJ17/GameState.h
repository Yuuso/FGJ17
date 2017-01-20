
#pragma once

#include <ExtendedEngine/State.h>


namespace spehs{ namespace audio{ class ActiveSoundSource; } class GameObject; }

class GameState : public extspehs::VisualState
{
public:
	GameState();
	~GameState();

	void init();
	bool update();

private:
	spehs::audio::ActiveSoundSource* sound;
	spehs::GameObject* object;
};

