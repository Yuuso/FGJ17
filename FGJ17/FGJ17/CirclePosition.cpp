
#include "CirclePosition.h"

#include <SpehsEngine/Time.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>

#include <glm/geometric.hpp>


#define PI 3.14159265358f


const float HORIZONT_DISTANCE(100.0f);//?
extern const float rotationToPosition;


CirclePosition::CirclePosition()
{
}
CirclePosition::CirclePosition(spehs::GameObject& _owner) : spehs::Component(_owner)
{

}
CirclePosition::~CirclePosition()
{
}

void CirclePosition::update()
{
	position += velocity * spehs::time::getDeltaTimeAsSeconds();

	spehs::Transform2D* transform = ownerObject->getComponent<spehs::Transform2D>();
	assert(transform);

	float x = position.x;
	if (position.x < PI * 0.5f)
	{
		if (spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition > PI)
		{
			x = position.x + 2 * PI;
		}
	}
	else if (position.x > PI * 1.5f)
	{
		if (spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition < PI)
		{
			x = position.x - 2 * PI;
		}
	}

	transform->setPosition(glm::vec2(x * rotationToPosition, position.y * (1 / position.z) + spehs::getActiveBatchManager()->getCamera2D()->position.y * (HORIZONT_DISTANCE / position.z)));
	transform->setScale(1 / position.z);
}

void CirclePosition::setPosition(const glm::vec3& _pos)
{
	position = _pos;
}
void CirclePosition::setVelocity(const glm::vec3& _pos)
{
	velocity = _pos;
}