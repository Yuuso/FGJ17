
#include "GameState.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/TextureManager.h>

#define PI 3.14159265358f


extern const float rotationToPosition = 10000.0f;


GameState::GameState()
{

}
GameState::~GameState()
{

}

void GameState::init()
{
	VisualState::init();
	spehs::getMainWindow()->clearColor(0.3f, 0.5f, 0.7f, 1.0f);

	textureManager->preloadTexture("Textures/bird.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
}

bool GameState::update()
{
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}

	//Camera
	static const float movementSpeed = 0.03f;
	static float cameraRotation = 0.0f; //0 - 2PI
	
	if (inputManager->isKeyDown(KEYBOARD_A))
	{
		cameraRotation -= movementSpeed * spehs::time::getDeltaTimeAsSeconds();
	}
	if (inputManager->isKeyDown(KEYBOARD_D))
	{
		cameraRotation += movementSpeed * spehs::time::getDeltaTimeAsSeconds();
	}

	if (cameraRotation > 2.0f * PI)
		cameraRotation = 0.0f;
	if (cameraRotation < 0.0f)
		cameraRotation = 2.0f * PI;

	camera->position = glm::vec2(cameraRotation * rotationToPosition, 0.0f);
	camera->update();

	environment.update();
	
	return true;
}