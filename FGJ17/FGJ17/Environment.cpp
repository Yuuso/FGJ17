
#include "Environment.h"

#include "CirclePosition.h"
#include "ObjectCreator.h"
#include "GameState.h"
#include "TargetLock.h"
#include "Cloud.h"

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

#include <algorithm>
#include <iostream>


#define PI 3.14159265358f


extern const float rotationToPosition;
glm::vec3 Environment::seaColor;
float timerCounter(0.0f);
const int backgroundDepth = -10000;
float Environment::sunPosition = 0.0f;
float Environment::light = 1.0f;
int Environment::day = 1;

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
	spehs::TextureData* textureData = textureManager->getTextureData("Textures/stars.png", spehs::TextureFiltering::Linear, spehs::TextureFiltering::Linear);
	stars->setTexture(textureData);
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
	seaColor = glm::vec3(0.2f + 0.8f * sin(sin(light)) * std::powf(cos(sin(light)), 3.0f), 0.6f * std::powf(light, 0.35f), 0.08f + 0.92f * light);
	sea->setColor(seaColor);
	seaShade->setColor(0, 0, unsigned char(128 * light), 255);
	seaShade->worldVertexArray[0].color.rgba.a = 0.0f;
	seaShade->worldVertexArray[1].color.rgba.a = 0.0f;

	sky->setColor(unsigned char(160 * std::powf(light, 0.27f)), unsigned char(200 * std::powf(light, 0.5f)), unsigned char(255 * std::powf(light, 1.1f)), 255);
	skyShade->setColor(128 + unsigned char(127 * std::powf(light, 0.1f)), 64 + unsigned char(160 * std::powf(light, 0.8f)), 128 + unsigned char(127 * light), unsigned char(255 * std::powf(light, 0.7f)));
	skyShade->worldVertexArray[2].color.rgba.a = 0.0f;
	skyShade->worldVertexArray[3].color.rgba.a = 0.0f;
//	stars->setColorAlpha((float)std::powf(1.0f - light, 1.5f));
}

Environment::Environment() : fingerCooldown(0.0f)
{
	float factorX = float(applicationData->getWindowHeight()) / 1920.0f;
	backgroundLeft = new Background(-4000.0f);
	backgroundCenter = new Background(4000.0f);
	backgroundRight = new Background(12000.0f);

	trumpet = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 2.0f, applicationData->getWindowWidth() / 2.0f);
	trumpet->setTexture(textureManager->getTextureData("Textures/trump.png"));
	trumpet->setPlaneDepth(30000);

	fingerBase = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 4.0f, applicationData->getWindowWidth() / 2.0f);
	fingerBase->setTexture(textureManager->getTextureData("Textures/finger_base.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
	fingerBase->setPlaneDepth(30001);
	fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, -applicationData->getWindowHeight());
	fingerHighlight = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 4.0f, applicationData->getWindowWidth() / 2.0f);
	fingerHighlight->setTexture(textureManager->getTextureData("Textures/finger_highlight.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
	fingerHighlight->setPlaneDepth(30002);
	fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, -applicationData->getWindowHeight());

	const int start = 0;
	for (unsigned i = 0; i < start; i++)
	{
		waves.push_back(ObjectCreator::createWaves());
		waves.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/waves.png"));
		waves.back()->getComponent<spehs::Sprite>()->setSize(spehs::rng::frandom(2300, 2800), spehs::rng::frandom(700, 900));
		waves.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-750.0f, -550.0f), spehs::rng::frandom(0.3f, 7.0f)));
		waves.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3((start - i) / float(start)));
	}

	for (unsigned i = 0; i < 60; i++)
	{
		clouds.push_back(ObjectCreator::createCloud());
		glm::vec2 size(spehs::rng::frandom(2300, 2800), spehs::rng::frandom(700, 900));
		clouds.back()->getComponent<Cloud>()->baseSprite->setSize(size.x, size.y);
		clouds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(900, 1100), 3.0f + std::powf(spehs::rng::frandom(0.0f, 1.0f), 0.7f) * spehs::rng::frandom(0.0f, 70.0f)));
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
	sunPosition += spehs::time::getDeltaTimeAsSeconds() * TWO_PI / DAY_CYCLE_SECONDS;
	while (sunPosition > TWO_PI)
	{
		sunPosition -= TWO_PI;
		day++;
	}
	light = std::powf(std::max(0.0f, sin(Environment::sunPosition)), 0.8f);
	backgroundLeft->update();
	backgroundCenter->update();
	backgroundRight->update();
	Cloud::baseColor = glm::vec3(std::powf(light, 0.9f) * 0.85f, std::powf(light, 1.1f) * 0.85f, std::powf(light, 1.3f) * 1.0f);
	Cloud::highlightColor = glm::vec3(std::powf(light, 0.4f) * 0.85f, std::powf(light, 0.7f) * 0.85f, std::powf(light, 0.9f) * 1.0f);

	if (sin(sunPosition + TWO_PI / DAY_CYCLE_SECONDS * 2.0f/*Seconds spawning begins/ends in advance*/) < 0.2f)
	{
		timerCounter -= spehs::time::getDeltaTimeAsSeconds();
		if (timerCounter < 0.0f)
		{
			timerCounter = DAY_CYCLE_SECONDS * 0.5f / (70.0f / std::powf(float(day + 3), 1.1f));

			for (unsigned i = 0; i < 10; i++)
			{
				birds.push_back(ObjectCreator::createBird());
				birds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-MAX_SPAWN_DISTANCE, MAX_SPAWN_DISTANCE), 10.0f));
				birds.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 0.0f, spehs::rng::frandom(-1.5f, -0.5f)));
				birds.back()->update();
			}
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
	
	//THE FINGER
	fingerCooldown -= spehs::time::getDeltaTimeAsSeconds();
	if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
	{
		fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerBase->getPosition().y, (float)-applicationData->getWindowHeightHalf() / 2.0f, 0.2f));
		fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerHighlight->getPosition().y, (float)-applicationData->getWindowHeightHalf() / 2.0f, 0.2f));
		if (fingerCooldown <= 0.0f)
		{
			fingerCooldown = 0.125f;
			for (unsigned i = 0; i < birds.size(); i++)
			{
				float angleToBird = birds[i]->getComponent<CirclePosition>()->getPosition().x - GameState::instance->getCamera().position.x / rotationToPosition;
				while (abs(angleToBird) > PI)
					angleToBird -= TWO_PI * /*sign*/(abs(angleToBird) / angleToBird);
				if (abs(angleToBird) < PI * 0.1f)
				{//Bird within field of view
					if (birds[i]->getComponent<TargetLock>() == nullptr)
						birds[i]->addComponent<TargetLock>();
				}
			}
		}
	}
	else
	{
		fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerBase->getPosition().y, (float)-applicationData->getWindowHeight(), 0.2f));
		fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerHighlight->getPosition().y, (float)-applicationData->getWindowHeight(), 0.2f));
	}
	fingerBase->setColor(glm::vec3(0.2f * std::powf(light, 0.6f), 0.25f * light, 0.22f * light));
	fingerHighlight->setColor(glm::vec3(0.6f + 0.3f * light, std::powf(light, 0.7f), 0.15f + 0.6f * std::powf(light, 0.85f)));

	trumpet->setPosition((inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) + spehs::getActiveBatchManager()->getCamera2D()->position.x, spehs::lerp(trumpet->getPosition().y, (float)-applicationData->getWindowHeightHalf() * 1.2f, 0.2f));
	trumpet->setColor(glm::vec3(0.4f + 0.6f * light, std::powf(light, 0.75f), 0.15f + 0.85f * std::powf(light, 1.5f)));


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
		fishes[i]->getComponent<spehs::Sprite>()->setColor(glm::vec3(1.0f, 0.25f + 0.75f * light, light));
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
		potatos[i]->getComponent<spehs::Sprite>()->setColor(glm::vec3(1.0f, std::powf(light, 0.5f), 0.4f + 0.6f * light));
		potatos[i]->getComponent<spehs::Transform2D>()->setRotation(potatos[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f * (i % 2 == 0 ? -1.0f : 1.0f));

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

	for (unsigned i = 0; i < clouds.size(); i++)
	{
		clouds[i]->getComponent<Cloud>()->baseSprite->setDepth(-clouds[i]->getComponent<CirclePosition>()->getPosition().z);
		clouds[i]->update();
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
