
#include "GameState.h"

#include <SpehsEngine/InputManager.h>


GameState::GameState()
{

}
GameState::~GameState()
{

}

void GameState::init()
{
	VisualState::init();

}

bool GameState::update()
{
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}
	
	return true;
}