
#include "ObjectCreator.h"

#include "Position3D.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/Sprite.h>


spehs::GameObject* ObjectCreator::createObject()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->addComponent<Position3D>();

	return result;
}