
#include "WaveMotion.h"

#include "CirclePosition.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/RNG.h>


WaveMotion::WaveMotion()
{
}
WaveMotion::WaveMotion(spehs::GameObject& _owner) : spehs::Component(_owner)
{
	value = spehs::rng::frandom(0.0f, 2.5f);
}
WaveMotion::~WaveMotion()
{
}


void WaveMotion::update()
{
	pos = ownerObject->getComponent<CirclePosition>()->getPosition();
	value += spehs::time::getDeltaTimeAsSeconds();
	ownerObject->getComponent<CirclePosition>()->setPosition(pos + glm::vec3(sin(value) * 0.001f, cos(value * 0.8f), sin(value * 0.1f) * 0.0001f));
}

void WaveMotion::setValue(const float _value)
{
	value = _value;
}