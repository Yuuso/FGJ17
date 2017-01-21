
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


#define PI 3.14159265358f


const float SPAWN_RADIUS(100.0f);
const float SPAWN_TIMER(1.0f);
float timerCounter(0.0f);


Environment::Environment()
{
	float factorX = float(applicationData->getWindowHeight()) / 1920.0f;
	background = spehs::Polygon::create(4, -10, 8000, 1920 * factorX);
	background->setTexture(textureManager->getTextureData("Textures/background.png"));
	background->setPosition(glm::vec2(4000.0f, 0.0f));
	background2 = spehs::Polygon::create(4, -10, 8000, 1920 * factorX);
	background2->setTexture(textureManager->getTextureData("Textures/background.png"));
	background2->setPosition(glm::vec2(-4000.0f, 0.0f));
	background3 = spehs::Polygon::create(4, -10, 8000, 1920 * factorX);
	background3->setTexture(textureManager->getTextureData("Textures/background.png"));
	background3->setPosition(glm::vec2(12000.0f, 0.0f));
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
			objects.push_back(ObjectCreator::createBird());
			objects.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-100.0f, 100.0f), 10.0f));
			objects.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 0.0f, spehs::rng::frandom(-1.5f, 0.5f)));
		}
	}
	
	for (unsigned i = 0; i < objects.size(); i++)
	{
		objects[i]->update();

		if (objects[i]->getComponent<CirclePosition>()->getPosition().z < 0.1f)
		{
			delete objects[i];
			objects[i] = objects.back();
			objects.pop_back();
		}
	}
}
