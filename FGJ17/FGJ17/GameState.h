
#pragma once

#include <ExtendedEngine/State.h>


namespace spehs{ class Polygon; class Text; }
class Environment;
class GameState : public extspehs::VisualState
{
public:
public:
	GameState();
	~GameState() override;

	void init() override;
	bool update() override;

private:
	Environment* environment = nullptr;
	spehs::Polygon* fader;
	spehs::Polygon* endscreen;
	spehs::Text* endText;
	float textTimer = 3.0f;
};

