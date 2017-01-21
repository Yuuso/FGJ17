
#pragma once

#include <ExtendedEngine/State.h>


class Environment;
class GameState : public extspehs::VisualState
{
public:
	static GameState* instance;
public:
	GameState();
	~GameState() override;

	void init() override;
	bool update() override;

private:
	Environment* environment = nullptr;
};

