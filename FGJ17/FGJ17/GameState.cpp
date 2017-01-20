
#include "GameState.h"

#include <SpehsEngine/SoundSource.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/AudioManager.h>
#include <SpehsEngine/AudioEngine.h>
#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/AnimatedSprite.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/TextureManager.h>

#include <glm/vec2.hpp>


GameState::GameState()
{
}
GameState::~GameState()
{
	delete sound;
}

void GameState::init()
{
	VisualState::init();

	sound = new spehs::audio::ActiveSoundSource;
	sound->setSound(spehs::AudioManager::instance->loadWAVE("test.wav"));

	object = new spehs::GameObject();
	object->addComponent<spehs::Transform2D>();
	object->addComponent<spehs::AnimatedSprite>();
	object->getComponent<spehs::AnimatedSprite>()->setTextureID(textureManager->preloadTexture("test.png"));
	object->getComponent<spehs::AnimatedSprite>()->setAnimation(glm::ivec2(30, 30), 2, 2, 4);
	object->getComponent<spehs::AnimatedSprite>()->setSize(200.0f, 200.0f);
}

bool GameState::update()
{
	if (inputManager->isKeyPressed(KEYBOARD_RETURN))
	{
		sound->play();
	}
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}

	object->update();

	return true;
}