
#include "Environment.h"

#include "CirclePosition.h"
#include "ObjectCreator.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Sprite.h>
#include <SpehsEngine/Mathematics.h>

#include <iostream>


#define PI 3.14159265358f


extern const float rotationToPosition;

const float SPAWN_RADIUS(100.0f);
const float SPAWN_TIMER(1.0f);
float timerCounter(0.0f);


Environment::Environment()
{
	float factorX = float(applicationData->getWindowHeight()) / 1920.0f;
	int backgroundDepth = -10000;
	background = spehs::Polygon::create(4, backgroundDepth, 8000, 1920 * factorX);
	background->setTexture(textureManager->getTextureData("Textures/background.png"));
	background->setPosition(glm::vec2(4000.0f, 0.0f));
	background2 = spehs::Polygon::create(4, backgroundDepth, 8000, 1920 * factorX);
	background2->setTexture(textureManager->getTextureData("Textures/background.png"));
	background2->setPosition(glm::vec2(-4000.0f, 0.0f));
	background3 = spehs::Polygon::create(4, backgroundDepth, 8000, 1920 * factorX);
	background3->setTexture(textureManager->getTextureData("Textures/background.png"));
	background3->setPosition(glm::vec2(12000.0f, 0.0f));

	trumpet = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 2.0f, applicationData->getWindowWidth() / 2.0f);
	trumpet->setTexture(textureManager->getTextureData("Textures/trump.png"));
	trumpet->setPlaneDepth(30000);

	finger = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 4.0f, applicationData->getWindowWidth() / 2.0f);
	finger->setTexture(textureManager->getTextureData("Textures/finger.png"));
	finger->setPlaneDepth(30000);
	finger->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x - applicationData->getWindowWidthHalf() / 2.0f, -applicationData->getWindowHeight());

	const int start = 250;
	for (unsigned i = 0; i < start; i++)
	{
		waves.push_back(ObjectCreator::createWaves());
		waves.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/waves.png"));
		waves.back()->getComponent<spehs::Sprite>()->setSize(spehs::rng::frandom(2300, 2800), spehs::rng::frandom(700, 900));
		waves.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-750.0f, -550.0f), spehs::rng::frandom(0.3f, 7.0f)));
		waves.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3((start - i) / float(start)));
	}
}
Environment::~Environment()
{
}

void Environment::update()
{
	timerCounter -= spehs::time::getDeltaTimeAsSeconds();
	if (timerCounter < 0.0f)
	{
		timerCounter = SPAWN_TIMER;

		for (unsigned i = 0; i < 10; i++)
		{
			birds.push_back(ObjectCreator::createBird());
			birds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-100.0f, 100.0f), 10.0f));
			birds.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 0.0f, spehs::rng::frandom(-1.5f, -0.5f)));
			birds.back()->update();
		}
	}


	if (inputManager->isKeyPressed(MOUSEBUTTON_LEFT))
	{
		potatos.push_back(ObjectCreator::createPotato());
		potatos.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/potato.png"));
		potatos.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3(spehs::rng::frandom(0.75f, 1.0f)));
		potatos.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition + (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) / rotationToPosition, 0.0, 0.2f));
		potatos.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 100.0f, spehs::rng::frandom(0.5f, 4.0f)));
		potatos.back()->getComponent<spehs::Transform2D>()->setRotation(spehs::rng::frandom(0.0f, 6.0f));
		potatos.back()->update();

		trumpet->setPosition(trumpet->getPosition().x, -applicationData->getWindowHeightHalf() * 0.3f);
	}

	if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
	{
		finger->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x - applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(finger->getPosition().y, (float) -applicationData->getWindowHeightHalf() / 2.0f, 0.2f));
	}
	else
	{
		finger->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x - applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(finger->getPosition().y, (float) -applicationData->getWindowHeight(), 0.2f));
	}

	trumpet->setPosition((inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) + spehs::getActiveBatchManager()->getCamera2D()->position.x, spehs::lerp(trumpet->getPosition().y, (float)-applicationData->getWindowHeightHalf() * 1.2f, 0.2f));


	if (!spehs::rng::irandom(0, 5))
	{
		fishes.push_back(ObjectCreator::createFish());
		fishes.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/fish.png"));
		fishes.back()->getComponent<spehs::Sprite>()->setSize(80.0f, 80.0f);
		fishes.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), -500.0f, spehs::rng::frandom(0.5f, 6.0f)));
		fishes.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(spehs::rng::frandom(-0.1f, 0.1f), 400.0f, spehs::rng::frandom(0.0f, 0.1f)));
		fishes.back()->getComponent<spehs::Transform2D>()->setRotation(spehs::rng::frandom(0.0f, 6.0f));
		fishes.back()->update();
	}

	for (int i = 0; i < fishes.size(); i++)
	{
		fishes[i]->getComponent<CirclePosition>()->setVelocity(fishes[i]->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -600.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f));
		fishes[i]->update();
		fishes[i]->getComponent<spehs::Sprite>()->setDepth(-fishes[i]->getComponent<CirclePosition>()->getPosition().z);
		fishes[i]->getComponent<spehs::Transform2D>()->setRotation(fishes[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f);

		if (fishes[i]->getComponent<CirclePosition>()->getPosition().y < -1000.0f)
		{
			delete fishes[i];
			fishes[i] = fishes.back();
			fishes.pop_back();
			i--;
		}
	}

	
	for (int i = 0; i < potatos.size(); i++)
	{
		potatos[i]->getComponent<CirclePosition>()->setVelocity(potatos[i]->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -50000.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f) * 0.95f * spehs::time::getDeltaTimeAsSeconds());

		potatos[i]->update();
		potatos[i]->getComponent<spehs::Sprite>()->setDepth(-potatos[i]->getComponent<CirclePosition>()->getPosition().z);
		potatos[i]->getComponent<spehs::Transform2D>()->setRotation(potatos[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f);

		if (potatos[i]->getComponent<CirclePosition>()->getPosition().y < -1000.0f)
		{
			delete potatos[i];
			potatos[i] = potatos.back();
			potatos.pop_back();
			i--;
		}
	}


	for (unsigned i = 0; i < waves.size(); i++)
	{
		waves[i]->update();
		waves[i]->getComponent<spehs::Sprite>()->setDepth(-waves[i]->getComponent<CirclePosition>()->getPosition().z);
	}

	
	for (int i = 0; i < birds.size(); i++)
	{
		birds[i]->update();
		birds[i]->getComponent<spehs::Sprite>()->setDepth(-birds[i]->getComponent<CirclePosition>()->getPosition().z);

		if (birds[i]->getComponent<CirclePosition>()->getPosition().z < 0.1f)
		{
			delete birds[i];
			birds[i] = birds.back();
			birds.pop_back();
			i--;
		}
	}
}
