
#include "Environment.h"

#include "CirclePosition.h"
#include "ObjectCreator.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/RNG.h>


#define PI 3.14159265358f


const float SPAWN_RADIUS(100.0f);
const float SPAWN_TIMER(1.0f);
float timerCounter(0.0f);


Environment::Environment()
{
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
