
#include "GameState.h"

#include "Environment.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/TextureManager.h>

#include <iostream>

#define PI 3.14159265358f


extern const float rotationToPosition = 1273.239544735164f;


GameState::GameState()
{

}
GameState::~GameState()
{
	if (environment)
	{
		delete environment;
	}
}

void GameState::init()
{
	VisualState::init();
	spehs::getMainWindow()->clearColor(0.3f, 0.5f, 0.7f, 1.0f);

	textureManager->preloadTexture("Textures/bird.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
	textureManager->preloadTexture("Textures/background.png", spehs::TextureFiltering::Linear, spehs::TextureFiltering::Linear);

	environment = new Environment;
}

bool GameState::update()
{
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}

	//Camera
	static const float movementSpeed = 0.5f;
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

	std::cout << cameraRotation << std::endl;

	camera->position = glm::vec2(cameraRotation * rotationToPosition, 0.0f);
	camera->update();

	environment->update();
	
	return true;
}