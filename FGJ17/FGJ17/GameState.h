
#pragma once

#include "Environment.h"

#include <ExtendedEngine/State.h>


class GameState : public extspehs::VisualState
{
public:
	GameState();
	~GameState() override;

	void init() override;
	bool update() override;

private:
	Environment environment;
};

