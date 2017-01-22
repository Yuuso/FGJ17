
#include "GameState.h"

#include "Environment.h"

#include <ExtendedEngine/ApplicationManager.h>

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/AudioEngine.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Mathematics.h>
#include <SpehsEngine/Text.h>

#include <glm/vec2.hpp>

#include <iostream>
#include <thread>
#include <chrono>

#define PI 3.14159265358f


extern const float rotationToPosition = 1273.239544735164f;
extern const float SOUNDFACTOR = 1000.0f;

GameState::GameState()
{
}
GameState::~GameState()
{
	if (environment)
	{
		delete environment;
	}

	fader->destroy();
	endscreen->destroy();
	endText->destroy();
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
	textureManager->preloadTexture("Textures/hitmarker.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest);

	environment = new Environment;

	fader = spehs::Polygon::create(4, INT16_MAX - 1, applicationData->getWindowWidth(), applicationData->getWindowHeight());
	fader->setColor(spehs::BLACK);
	fader->setColorAlpha(1.0f);
	fader->setPosition(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf());
	fader->setCameraMatrixState(false);

	endscreen = spehs::Polygon::create(4, INT16_MAX, applicationData->getWindowWidth(), applicationData->getWindowHeight());
	endscreen->setTexture(textureManager->getTextureData("Textures/crow.png"));
	endscreen->setPosition(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf());
	endscreen->setCameraMatrixState(false);
	endscreen->setColorAlpha(0.0f);

	endText = spehs::Text::create("", INT16_MAX);
	endText->setFont(applicationData->GUITextFontPath, 24);
	endText->setCameraMatrixState(false);
	endText->setColor(0.6f, 0.6f, 0.65f);
	endText->setPosition(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf());
	endText->setAlpha(0.0f);

	
	credits.push_back(spehs::Text::create("CREDITS", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, 0.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("Juuso", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 60.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("\tProgramming, Graphics", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 30.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("Teo", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 45.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("\tGraphics, Programming", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 30.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("Jussi-Pekka", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 45.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("\tLore, Narration, Graphics", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 30.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("Ossi", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 45.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("\tSound, Music", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 30.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("Tatu", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 45.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);
	credits.push_back(spehs::Text::create("\tSound, Music", INT16_MAX));
	credits.back()->setFont(applicationData->GUITextFontPath, 20);
	credits.back()->setPosition(applicationData->getWindowWidthHalf() / 6.0f, credits.end()[-2]->getPosition().y - 30.0f);
	credits.back()->setColor(0.6f, 0.6f, 0.65f);
	credits.back()->setAlpha(0.0f);


	spehs::audio::AudioEngine::setListenerGain(applicationData->masterVolume);
}

bool GameState::update()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}

	if ((inputManager->isKeyDown(KEYBOARD_LCTRL) || inputManager->isKeyDown(KEYBOARD_RCTRL)) && inputManager->isKeyDown(KEYBOARD_R))
	{
		extspehs::ApplicationManager::setState(new GameState);
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
		cameraRotation += spehs::time::getDeltaTimeAsSeconds() * (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) * 0.001f;
	}
	
	if (cameraRotation > 2.0f * PI)
		cameraRotation = 0.0f;
	if (cameraRotation < 0.0f)
		cameraRotation = 2.0f * PI;

	if (!environment->getDead())
	{
		camera->position = glm::vec2(cameraRotation * rotationToPosition, 0.0f);
		fader->setColorAlpha(spehs::lerp(fader->getColorAlpha(), 0.0f, 0.1f * spehs::time::getDeltaTimeAsSeconds()));
	}
	else
	{
		fader->setColorAlpha(spehs::lerp(fader->getColorAlpha(), 1.0f, 0.13f * spehs::time::getDeltaTimeAsSeconds()));

		if (textTimer < 0.0f)
		{
			if (environment->score == 1)
			{
				endText->setString("You survived for 1 day...");
			}
			else
			{
				endText->setString("You survived for " + std::to_string(environment->score) + " days...");
			}
			endText->setAlpha(spehs::lerp(endText->getColor().a, 1.0f, 0.1f * spehs::time::getDeltaTimeAsSeconds()));
		}
		else
		{
			textTimer -= spehs::time::getDeltaTimeAsSeconds();
		}

		if (fader->getColorAlpha() > 0.95f)
		{
			endscreen->setColorAlpha(spehs::lerp(endscreen->getColorAlpha(), 1.0f, 0.11f * spehs::time::getDeltaTimeAsSeconds()));

			for (unsigned i = 0; i < credits.size(); i++)
			{
				credits[i]->setPosition(credits[i]->getPosition() + glm::vec2(0.0f, 40.0f * spehs::time::getDeltaTimeAsSeconds()));
				credits[i]->setAlpha(1.0f - abs((credits[i]->getPosition().y - applicationData->getWindowHeightHalf()) / applicationData->getWindowHeightHalf()));
			}
		}
	}
	camera->update();

	environment->update();

	spehs::audio::AudioEngine::setListenerPosition(camera->position / SOUNDFACTOR);
	
	return true;
}