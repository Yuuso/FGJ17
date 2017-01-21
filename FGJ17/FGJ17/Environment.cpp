
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
const int backgroundDepth = -10000;
float Environment::sunPosition = 0.0f;

Environment::Background::Background(const float x)
{
	sea = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth, 8000, applicationData->getWindowHeightHalf());
	sea->setColor(0, 200, 255, 255);
	sea->setPosition(x, -applicationData->getWindowHeightHalf());
	seaShade = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth + 1, 8000, applicationData->getWindowHeightHalf());
	seaShade->setPosition(sea->getX(), sea->getY());
	sky = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth, 8000, applicationData->getWindowHeightHalf());
	sky->setColor(150, 200, 255, 255);
	sky->setPosition(x, 0.0f);
	skyShade = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth + 2, 8000, applicationData->getWindowHeightHalf());
	skyShade->setPosition(sky->getX(), sky->getY());
	stars = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth + 1, 8000.0f, 540.0f);
	stars->setPosition(sky->getX(), sky->getY());
	stars->setTexture("Textures/stars.png");
}
Environment::Background::~Background()
{
	sea->destroy();
	seaShade->destroy();
	sky->destroy();
	skyShade->destroy();
	stars->destroy();
}
void Environment::Background::update()
{
	const float light = (sin(Environment::sunPosition) + 1.0f ) * 0.5f;
	sea->setColor(0, 0, unsigned char(255 * light), 255);
	seaShade->setColor(0, 0, unsigned char(128 * light), 255);
	seaShade->worldVertexArray[0].color.rgba.a = 0.0f;
	seaShade->worldVertexArray[1].color.rgba.a = 0.0f;
	sky->setColor(unsigned char(150 * light), unsigned char(200 * light), unsigned char(255 * light), 255);
	skyShade->setColor(128 + unsigned char(127 * sin(Environment::sunPosition)), 128 + unsigned char(127 * sin(Environment::sunPosition)), 128 + unsigned char(127 * sin(Environment::sunPosition)), 255);
	skyShade->worldVertexArray[2].color.rgba.a = 0.0f;
	skyShade->worldVertexArray[3].color.rgba.a = 0.0f;
	//stars->setColorAlpha((float)std::powf(1.0f - light, 1.5f));
}

Environment::Environment()
{
	float factorX = float(applicationData->getWindowHeight()) / 1920.0f;
	backgroundLeft = new Background(-4000.0f);
	backgroundCenter = new Background(4000.0f);
	backgroundRight = new Background(12000.0f);

	trumpet = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 2.0f, applicationData->getWindowWidth() / 2.0f * factorX);
	trumpet->setTexture(textureManager->getTextureData("Textures/trump.png"));
	trumpet->setPlaneDepth(30000);

	const int start = 250;
	for (unsigned i = 0; i < start; i++)
	{
		waves.push_back(ObjectCreator::createWaves());
		waves.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/waves.png"));
		waves.back()->getComponent<spehs::Sprite>()->setSize(spehs::rng::frandom(1900, 2100), spehs::rng::frandom(500, 600));
		waves.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-550.0f, -350.0f), spehs::rng::frandom(0.3f, 7.0f)));
		waves.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3((start - i) / float(start)));
	}
}
Environment::~Environment()
{
	delete backgroundLeft;
	delete backgroundCenter;
	delete backgroundRight;
}

void Environment::update()
{
	sunPosition += spehs::time::getDeltaTimeAsSeconds() * TWO_PI / 20.0f/*Total day length in seconds*/;
	while (sunPosition > TWO_PI)
		sunPosition -= TWO_PI;
	backgroundLeft->update();
	backgroundCenter->update();
	backgroundRight->update();

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
		potatos.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition + (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) / rotationToPosition, 0.0, 0.3f));
		potatos.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 100.0f, 1.0f));
		potatos.back()->getComponent<spehs::Transform2D>()->setRotation(spehs::rng::frandom(0.0f, 6.0f));
		potatos.back()->update();

		trumpet->setPosition(trumpet->getPosition().x, -applicationData->getWindowHeightHalf() * 0.5f);
	}

	trumpet->setPosition((inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) + spehs::getActiveBatchManager()->getCamera2D()->position.x, spehs::lerp(trumpet->getPosition().y, (float)-applicationData->getWindowHeightHalf(), 0.2f));

	
	for (unsigned i = 0; i < potatos.size(); i++)
	{
		potatos[i]->getComponent<CirclePosition>()->setVelocity(potatos[i]->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -50000.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f) * 0.9f * spehs::time::getDeltaTimeAsSeconds());

		potatos[i]->update();
		potatos[i]->getComponent<spehs::Sprite>()->setDepth(-potatos[i]->getComponent<CirclePosition>()->getPosition().z);
		potatos[i]->getComponent<spehs::Transform2D>()->setRotation(potatos[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f);

		if (potatos[i]->getComponent<CirclePosition>()->getPosition().y < -100.0f)
		{
			delete potatos[i];
			potatos[i] = potatos.back();
			potatos.pop_back();
		}
	}


	for (unsigned i = 0; i < waves.size(); i++)
	{
		waves[i]->update();
		waves[i]->getComponent<spehs::Sprite>()->setDepth(-waves[i]->getComponent<CirclePosition>()->getPosition().z);
	}

	
	for (unsigned i = 0; i < birds.size(); i++)
	{
		birds[i]->update();
		birds[i]->getComponent<spehs::Sprite>()->setDepth(-birds[i]->getComponent<CirclePosition>()->getPosition().z);

		if (birds[i]->getComponent<CirclePosition>()->getPosition().z < 0.1f)
		{
			delete birds[i];
			birds[i] = birds.back();
			birds.pop_back();
		}
	}
}
