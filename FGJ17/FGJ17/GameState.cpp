
#include "GameState.h"

#include "Environment.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/ApplicationData.h>

#include <iostream>
#include <thread>
#include <chrono>

#define PI 3.14159265358f


extern const float rotationToPosition = 1273.239544735164f;

GameState* GameState::instance = nullptr;
GameState::GameState()
{
	if (!instance)
		instance = this;
}
GameState::~GameState()
{
	if (environment)
	{
		delete environment;
	}
	if (instance == this)
		instance = nullptr;
}

void GameState::init()
{
	VisualState::init();
	spehs::getMainWindow()->clearColor(0.3f, 0.5f, 0.7f, 1.0f);

	textureManager->preloadTexture("Textures/bird.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
	textureManager->preloadTexture("Textures/background.png", spehs::TextureFiltering::Linear, spehs::TextureFiltering::Linear);
	textureManager->preloadTexture("Textures/waves.png", spehs::TextureFiltering::Linear, spehs::TextureFiltering::Linear);
	textureManager->preloadTexture("Textures/potato.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
	textureManager->preloadTexture("Textures/trump.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
	textureManager->preloadTexture("Textures/finger.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);
	textureManager->preloadTexture("Textures/fish.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);

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
	if (abs(inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) > applicationData->getWindowWidthHalf() / 2.0f)
	{
		cameraRotation += spehs::time::getDeltaTimeAsSeconds() * (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) * 0.003f;
	}
	
	if (cameraRotation > 2.0f * PI)
		cameraRotation = 0.0f;
	if (cameraRotation < 0.0f)
		cameraRotation = 2.0f * PI;

	camera->position = glm::vec2(cameraRotation * rotationToPosition, 0.0f);
	camera->update();

	environment->update();
	
	return true;
}