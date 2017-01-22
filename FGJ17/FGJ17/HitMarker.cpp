
#include "HitMarker.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/Time.h>


HitMarker::HitMarker()
{
}
HitMarker::HitMarker(spehs::GameObject& _owner) : spehs::Component(_owner)
{

}
HitMarker::~HitMarker()
{
}


void HitMarker::update()
{
	ownerObject->getComponent<spehs::Transform2D>()->setScale(ownerObject->getComponent<spehs::Transform2D>()->getScale() + sizeSpeed * spehs::time::getDeltaTimeAsSeconds());
	ownerObject->getComponent<spehs::Transform2D>()->setRotation(ownerObject->getComponent<spehs::Transform2D>()->getRotation() + rotSpeed * spehs::time::getDeltaTimeAsSeconds());

	timer -= spehs::time::getDeltaTimeAsSeconds();
	if (timer <= 0)
	{
		pleaseDelete = true;
	}
}
