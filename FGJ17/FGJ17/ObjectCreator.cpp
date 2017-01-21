
#include "ObjectCreator.h"

#include "CirclePosition.h"
#include "WaveMotion.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/AnimatedSprite.h>
#include <SpehsEngine/TextureManager.h>


spehs::GameObject* ObjectCreator::createBird()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<CirclePosition>();
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::AnimatedSprite>();
	result->getComponent<spehs::AnimatedSprite>()->setTexture(textureManager->getTextureData("Textures/bird.png"));
	result->getComponent<spehs::AnimatedSprite>()->setAnimation(glm::ivec2(64, 32), 1, 5, 5);
	result->getComponent<spehs::AnimatedSprite>()->setAnimationSpeed(0.05f);
	result->addComponent<WaveMotion>();

	return result;
}
spehs::GameObject* ObjectCreator::createPotato()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->addComponent<CirclePosition>();

	return result;
}
spehs::GameObject* ObjectCreator::createWaves()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->addComponent<CirclePosition>();
	result->addComponent<WaveMotion>();

	return result;
}
spehs::GameObject* ObjectCreator::createFish()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->addComponent<CirclePosition>();

	return result;
}