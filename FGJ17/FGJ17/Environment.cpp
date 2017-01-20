
#include "Environment.h"

#include "Position3D.h"
#include "ObjectCreator.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/RNG.h>


#define PI 3.14159265358f


const float SPAWN_RADIUS(100.0f);
const float SPAWN_TIMER(0.5f);
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

		for (unsigned i = 0; i < 5; i++)
		{
			objects.push_back(ObjectCreator::createObject());
			float randomPlacer = spehs::rng::frandom(0.0f, 2 * PI);
			objects.back()->getComponent<Position3D>()->setPseudoCamera(&camera);
			objects.back()->getComponent<Position3D>()->setPosition(glm::vec3(sin(randomPlacer), spehs::rng::frandom(-10.0f, 10.0f), cos(randomPlacer)));
			objects.back()->getComponent<Position3D>()->setVelocityTowards(glm::vec3(0.0f));
		}
	}

	for (unsigned i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
}
