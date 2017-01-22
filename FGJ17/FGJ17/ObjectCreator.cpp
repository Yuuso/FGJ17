
#include "ObjectCreator.h"

#include "CirclePosition.h"
#include "WaveMotion.h"
#include "Cloud.h"
#include "HitMarker.h"

#include <SpehsEngine/RNG.h>
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
	result->getComponent<WaveMotion>()->setFactor(0.15f);

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
	result->getComponent<WaveMotion>()->setFactor(0.3f);

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
spehs::GameObject* ObjectCreator::createCloud()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();

	const int cloudIndex = spehs::rng::irandom(1, NUM_CLOUD_VARIATIONS);
	Cloud& cloud = *result->addComponent<Cloud>();
	cloud.baseSprite = result->addComponent<spehs::Sprite>();
	cloud.baseSprite->setTexture(textureManager->getTextureData("Textures/cloud_base" + std::to_string(cloudIndex) + ".png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
	cloud.createHighlight(cloudIndex);
	result->addComponent<CirclePosition>()->setVelocity(glm::vec3(std::powf(spehs::rng::frandom(0.0f, 1.0f), 0.5f) * spehs::rng::frandom(-0.01f, 0.01f), 0.0f, 0.0f));	

	return result;
}
spehs::GameObject* ObjectCreator::createHitMarker()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/hitmarker.png"));
	result->getComponent<spehs::Sprite>()->setSize(80.0f, 80.0f);
	result->getComponent<spehs::Sprite>()->setDepth(30000);
	result->addComponent<HitMarker>();

	return result;
}
spehs::GameObject* ObjectCreator::createBobber()
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::Sprite>();
	result->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/bobber.png"));
	result->getComponent<spehs::Sprite>()->setSize(80.0f, 80.0f);
	result->getComponent<spehs::Sprite>()->setDepth(30000);
	result->addComponent<CirclePosition>();
	result->addComponent<WaveMotion>();
	result->getComponent<WaveMotion>()->setFactor(0.5f);

	return result;
}