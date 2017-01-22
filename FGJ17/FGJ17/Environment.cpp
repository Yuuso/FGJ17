
#include "Environment.h"

#include "CirclePosition.h"
#include "ObjectCreator.h"
#include "GameState.h"
#include "TargetLock.h"
#include "Cloud.h"
#include "HitMarker.h"

#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Line.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Sprite.h>
#include <SpehsEngine/Mathematics.h>
#include <SpehsEngine/GUIRectangle.h>
#include <SpehsEngine/CommonColor.h>
#include <SpehsEngine/SoundSource.h>
#include <SpehsEngine/AudioManager.h>
#include <SpehsEngine/AudioEngine.h>

#include <algorithm>
#include <iostream>




//~~~~~~~~TODO~~~~~~~~
/*
- music
- credits
*/





#define PI 3.14159265358f


extern const float rotationToPosition;
const float MIN_LIGHT(0.01f);
const float RANDOM_MAGICAL_NIGHT_COLOR_VALUE(0.1f);
const float LOW_WATERS(500.0f);
const float BIRD_DAMAGE(0.05f);
const float BIRD_HEALING(0.5f);
const float FISH_SPAWN_FREQ(0.1f);
glm::vec3 seaColor;
float timerCounter(0.0f);
const int backgroundDepth = -10000;
float sunPosition = 0.0f;
float light = 1.0f;
int day = 1;

Environment::Background::Background(const float x)
{
	sea = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth, 8000, applicationData->getWindowHeightHalf());
	sea->setColor(0, 200, 255, 255);
	sea->setPosition(x, -applicationData->getWindowHeightHalf());

	seaShade = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth + 1, 8000, applicationData->getWindowHeightHalf());
	seaShade->setPosition(sea->getX(), sea->getY());

	sky = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth, 8000, applicationData->getWindowHeightHalf());
	sky->setColor(150, 200, 255, 255);
	sky->setPosition(x, 0.0f);

	skyShade = spehs::Polygon::create(spehs::Shape::BUTTON, backgroundDepth + 2, 8000, applicationData->getWindowHeightHalf());
	skyShade->setPosition(sky->getX(), sky->getY());

	stars = spehs::Polygon::create(4, backgroundDepth + 3, 8000, 540);
	stars->setPosition(x, 230.0f);
	stars->setTexture(textureManager->getTextureData("Textures/stars.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
}
Environment::Background::~Background()
{
	sea->destroy();
	seaShade->destroy();
	sky->destroy();
	skyShade->destroy();
	stars->destroy();
}
void Environment::Background::update()
{
	seaColor = glm::vec3(0.2f + 0.8f * sin(sin(light)) * std::powf(cos(sin(light)), 3.0f), 0.6f * std::powf(light, 0.35f), 0.08f + 0.92f * light);
	sea->setColor(seaColor);
	seaShade->setColor(0, 0, unsigned char(128 * light), 255);
	seaShade->worldVertexArray[0].color.rgba.a = 0.0f;
	seaShade->worldVertexArray[1].color.rgba.a = 0.0f;

	static const float settingSpeed = 0.005f;
	glm::vec4 colorOfTheSky = sky->getColor();
	if (light <= RANDOM_MAGICAL_NIGHT_COLOR_VALUE)
	{
		sky->setColor(spehs::lerp(colorOfTheSky, glm::vec4(0.0f, 0.0f, 0.1f, 1.0f), settingSpeed));
	}
	else
	{
		sky->setColor(spehs::lerp(colorOfTheSky, glm::vec4(unsigned char(160 * std::powf(light, 0.2f)) / 255.0f, unsigned char(200 * std::powf(light, 0.5f)) / 255.0f, unsigned char(255 * std::powf(light, 1.3f)) / 255.0f, 1.0f), settingSpeed));
	}
	skyShade->setColor(128 + unsigned char(127 * std::powf(light, 0.1f)), 64 + unsigned char(160 * std::powf(light, 0.8f)), 128 + unsigned char(127 * light), unsigned char(255 * std::powf(light, 0.7f)));
	skyShade->worldVertexArray[2].color.rgba.a = 0.0f;
	skyShade->worldVertexArray[3].color.rgba.a = 0.0f;
	stars->setColorAlpha((float)std::powf(1.0f - light, 1.5f));

}

Environment::Environment() : fingerCooldown(0.0f)
{
	day = 1;
	sunPosition = 0.0f;
	light = 1.0f;

	float factorX = float(applicationData->getWindowHeight()) / 1920.0f;
	backgroundLeft = new Background(-4000.0f);
	backgroundCenter = new Background(4000.0f);
	backgroundRight = new Background(12000.0f);

	trumpet = spehs::Polygon::create(4, 30000, applicationData->getWindowWidth() / 2.0f, applicationData->getWindowWidth() / 2.0f);
	trumpet->setTexture(textureManager->getTextureData("Textures/trump.png"));

	snare = spehs::Polygon::create(4, 30000, applicationData->getWindowHeight() * 0.15f, applicationData->getWindowHeight());
	snare->setTexture(textureManager->getTextureData("Textures/snare.png"));

	snareLine = spehs::Line::create(20000);
	snareLine->setColor(173, 203, 162);

	bobber = ObjectCreator::createBobber();

	snareLine->setRenderState(false);
	bobber->getComponent<spehs::Sprite>()->setRenderState(false);

	fingerBase = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 4.0f, applicationData->getWindowWidth() / 2.0f);
	fingerBase->setTexture(textureManager->getTextureData("Textures/finger_base.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
	fingerBase->setPlaneDepth(30001);
	fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, -applicationData->getWindowHeight());
	fingerHighlight = spehs::Polygon::create(4, backgroundDepth, applicationData->getWindowWidth() / 4.0f, applicationData->getWindowWidth() / 2.0f);
	fingerHighlight->setTexture(textureManager->getTextureData("Textures/finger_highlight.png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
	fingerHighlight->setPlaneDepth(30002);
	fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, -applicationData->getWindowHeight());

	const int start = 400;
	for (unsigned i = 0; i < start; i++)
	{
		waves.push_back(ObjectCreator::createWaves());
		waves.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/waves.png"));
		waves.back()->getComponent<spehs::Sprite>()->setSize(spehs::rng::frandom(2800, 3800), spehs::rng::frandom(500, 700));
		waves.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-LOW_WATERS + 150.0f, -LOW_WATERS + 300.0f), spehs::rng::frandom(0.5f, 6.0f)));
		waves.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3(std::max(0.2f,(start - i) / float(start))));
	}
	for (unsigned i = 0; i < start; i++)
	{
		waves.push_back(ObjectCreator::createWaves());
		waves.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/waves.png"));
		waves.back()->getComponent<spehs::Sprite>()->setSize(spehs::rng::frandom(3200, 4200), spehs::rng::frandom(300, 500));
		waves.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(-LOW_WATERS + 300.0f, -LOW_WATERS + 550.0f), spehs::rng::frandom(4.0f, 15.0f)));
		waves.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3(std::max(0.2f, (start - i) / float(start))));
	}

	for (unsigned i = 0; i < 60; i++)
	{
		clouds.push_back(ObjectCreator::createCloud());
		glm::vec2 size(spehs::rng::frandom(2300, 2800), spehs::rng::frandom(700, 900));
		clouds.back()->getComponent<Cloud>()->baseSprite->setSize(size.x, size.y);
		clouds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(900, 1100), 3.0f + std::powf(spehs::rng::frandom(0.0f, 1.0f), 0.7f) * spehs::rng::frandom(0.0f, 70.0f)));
	}

	
	foodBar = spehs::Polygon::create(4, 31000, 20.0f, 20.0f);
	foodBar->setColor(spehs::GREEN);
	foodBar->setCameraMatrixState(false);

	foodLogo = spehs::Polygon::create(4, 31000, 80.0f, 80.0f);
	foodLogo->setTexture(textureManager->getTextureData("Textures/foodlogo.png"));
	foodLogo->setPosition(applicationData->getWindowWidth() - 40.0f, applicationData->getWindowHeight() - 40.0f);
	foodLogo->setCameraMatrixState(false);

	ammunitionBar = spehs::Polygon::create(4, 31000, 20.0f, 20.0f);
	ammunitionBar->setColor(spehs::RED);
	ammunitionBar->setCameraMatrixState(false);

	ammunitionLogo = spehs::Polygon::create(4, 31000, 80.0f, 80.0f);
	ammunitionLogo->setTexture(textureManager->getTextureData("Textures/trumpetlogo.png"));
	ammunitionLogo->setPosition(applicationData->getWindowWidth() - 40.0f, applicationData->getWindowHeight() - 140.0f);
	ammunitionLogo->setCameraMatrixState(false);

	buyButton = new spehs::GUIRectangle(120.0f, 40.0f);
	buyButton->setDepth(31500);
	buyButton->setColor(spehs::commonColor(spehs::ORANGE));
	buyButton->setPositionGlobal(applicationData->getWindowWidthHalf() - buyButton->getWidth() / 2.0f, buyButton->getHeight());
	buyButton->setString("BUY POTATOES");
	buyButton->setJustification(GUIRECT_TEXT_JUSTIFICATION_CENTER);

	buyButton->updatePosition();
	buyButton->visualUpdate();
	buyButton->setPressCallback(std::bind(&Environment::buyPotato, this));


	casette = spehs::Polygon::create(4, 30000, 128, 64);
	casette->setTexture(textureManager->getTextureData("Textures/casette.png"));
	casette->setRenderState(false);
	casette->setCameraMatrixState(false);
	casette->setPosition(applicationData->getWindowWidth() - casette->getWidth(), applicationData->getWindowHeight() - 400.0f);

	speech = new spehs::audio::SoundSource;
	speech->setPriority(1);
	speech->setPitch(0.95f);

	waveSounds = new spehs::audio::SoundSource;
	waveSounds->setLooping(true);
	waveSounds->setGain(0.0f);
	waveSounds->setPriority(0);
	waveSounds->setSound(spehs::AudioManager::instance->loadWAVE("Sounds/aallot.wav"));
	waveSounds->play();
	
	spehs::AudioManager::instance->loadWAVE("Music/Day1.wav");
	spehs::AudioManager::instance->loadWAVE("Music/Day2.wav");
	spehs::AudioManager::instance->loadWAVE("Music/Night1.wav");
	spehs::AudioManager::instance->loadWAVE("Music/Night2.wav");
	music = new spehs::audio::SoundSource;
	music->setPriority(0);
	music->setSound("Music/Day1.wav");
	music->play();
	musicName = MusicName::Day1;
}

Environment::~Environment()
{
	delete backgroundLeft;
	delete backgroundCenter;
	delete backgroundRight;

	//todo delete rest...
	for (unsigned i = 0; i < birds.size(); i++)
	{
		delete birds[i];
	}
	for (unsigned i = 0; i < potatos.size(); i++)
	{
		delete potatos[i];
	}
	for (unsigned i = 0; i < waves.size(); i++)
	{
		delete waves[i];
	}
	for (unsigned i = 0; i < clouds.size(); i++)
	{
		delete clouds[i];
	}
	for (unsigned i = 0; i < fishes.size(); i++)
	{
		delete fishes[i];
	}
	for (unsigned i = 0; i < hitmarkers.size(); i++)
	{
		delete hitmarkers[i];
	}
	trumpet->destroy();
	snare->destroy();
	snareLine->destroy();
	delete bobber;
	delete caught;
	fingerBase->destroy();
	fingerHighlight->destroy();
	foodBar->destroy();
	foodLogo->destroy();
	ammunitionBar->destroy();
	ammunitionLogo->destroy();
	delete buyButton;
	casette->destroy();
	delete speech;
	delete music;
	delete waveSounds;
}

void Environment::update()
{
	if (!music->isPlaying())
		nextMusic();
	sfxManager.update();
	sunPosition += spehs::time::getDeltaTimeAsSeconds() * TWO_PI / DAY_CYCLE_SECONDS;
	while (sunPosition > TWO_PI)
	{
		sunPosition -= TWO_PI;
		day++;
	}
	light = std::powf(std::max(MIN_LIGHT, sin(sunPosition)), 0.8f);
	backgroundLeft->update();
	backgroundCenter->update();
	backgroundRight->update();
	Cloud::baseColor = glm::vec3(std::powf(light, 0.9f) * 0.85f, std::powf(light, 1.1f) * 0.85f, std::powf(light, 1.3f) * 1.0f);
	Cloud::highlightColor = glm::vec3(std::powf(light, 0.4f) * 0.85f, std::powf(light, 0.7f) * 0.85f, std::powf(light, 0.9f) * 1.0f);
	waveSounds->setGain(spehs::lerp(waveSounds->getGain(), 1.0f, 0.2f * spehs::time::getDeltaTimeAsSeconds()));

	if (food <= 0.0f || inputManager->isKeyPressed(KEYBOARD_K))
	{
		if (!dead)
		{
			sfxManager.playSound("Sounds/death.wav", spehs::audio::AudioEngine::getListenerPosition(), 1.0f, 1.0f, 4.0f);
			score = day - 1;

			foodBar->setRenderState(false);
			ammunitionBar->setRenderState(false);
			ammunitionLogo->setRenderState(false);
			foodLogo->setRenderState(false);
			buyButton->setRenderState(false);
			fingerBase->setRenderState(false);
			fingerHighlight->setRenderState(false);

			trumpet->setRenderState(false);
			snare->setRenderState(false);

			dead = true;
		}
		casette->setRenderState(false);
		snareLine->setRenderState(false);
		bobber->getComponent<spehs::Sprite>()->setRenderState(false);
		if (caught)
		{
			caught->getComponent<spehs::Sprite>()->setRenderState(false);
		}

		music->setGain(spehs::lerp(music->getGain(), 0.0f, 0.4f * spehs::time::getDeltaTimeAsSeconds()));
		speech->setGain(spehs::lerp(speech->getGain(), 0.0f, 0.4f * spehs::time::getDeltaTimeAsSeconds()));
	}

	timerCounter -= spehs::time::getDeltaTimeAsSeconds();
	if (sin(sunPosition + TWO_PI / DAY_CYCLE_SECONDS * 2.0f/*Seconds spawning begins/ends in advance*/) < 0.2f)
	{
		if (timerCounter < 0.0f)
		{
			timerCounter = 2.0f - std::min(1.9f, 0.1f * day);

			for (unsigned i = 0; i < 7 + day; i++)
			{
				birds.push_back(ObjectCreator::createBird());
				birds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(0.0f, MAX_SPAWN_HEIGHT), spehs::rng::frandom(7.5f, 10.0f)));
				birds.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 0.0f, spehs::rng::frandom(-1.5f, -0.5f)));
				birds.back()->update();
			}
		}
	}
	else
	{
		if (timerCounter < 0.0f)
		{
			timerCounter = 3.5f;

			for (unsigned i = 0; i < day; i++)
			{
				birds.push_back(ObjectCreator::createBird());
				birds.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), spehs::rng::frandom(0, MAX_SPAWN_HEIGHT), spehs::rng::frandom(8.5f, 10.0f)));
				birds.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(spehs::rng::frandom(-0.05f, 0.05f), 0.0f, 0.0f));
				birds.back()->update();
			}
		}
	}


	if (!dead)
	{
		buyButton->inputUpdate();

		foodBar->resize(food * 10.0f, foodBar->getHeight());
		foodBar->setPosition(applicationData->getWindowWidth() - 100.0f - foodBar->getWidth() / 2.0f, applicationData->getWindowHeight() - 40.0f);

		ammunitionBar->resize(ammunition * 3.0f, ammunitionBar->getHeight());
		ammunitionBar->setPosition(applicationData->getWindowWidth() - 100.0f - ammunitionBar->getWidth() / 2.0f, applicationData->getWindowHeight() - 140.0f);
	}

	

	if (!dead)
	{
		if (inputManager->isKeyPressed(MOUSEBUTTON_RIGHT))
		{
			tool = !tool;
			fishing = false;
			catchingFish = false;
			snareLine->setRenderState(false);
			bobber->getComponent<spehs::Sprite>()->setRenderState(false);
		}

		static const float toolSwitchSpeed = 30.0f;
		if (tool)
		{
			trumpet->setPosition(spehs::lerp(trumpet->getPosition(), trumpet->getPosition() + glm::vec2(0.0f, -applicationData->getWindowHeight()), toolSwitchSpeed * 1.4f * spehs::time::getDeltaTimeAsSeconds()));
			snare->setPosition(spehs::lerp(snare->getPosition(), glm::vec2((inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) + spehs::getActiveBatchManager()->getCamera2D()->position.x, spehs::lerp(snare->getPosition().y, (float) -applicationData->getWindowHeightHalf() * 0.5f, toolSwitchSpeed * spehs::time::getDeltaTimeAsSeconds())), toolSwitchSpeed * spehs::time::getDeltaTimeAsSeconds()));
			if (fishing)
			{
				snare->setRotation(spehs::lerp(snare->getRotation(), 0.15f, 2.0f * spehs::time::getDeltaTimeAsSeconds()));
			}
			else
			{
				snare->setRotation(spehs::lerp(snare->getRotation(), 0.0f, 2.0f * spehs::time::getDeltaTimeAsSeconds()));
			}
		}
		else
		{
			trumpet->setPosition(spehs::lerp(trumpet->getPosition(), glm::vec2((inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) + spehs::getActiveBatchManager()->getCamera2D()->position.x, spehs::lerp(trumpet->getPosition().y, (float) -applicationData->getWindowHeightHalf() * 1.2f, toolSwitchSpeed * spehs::time::getDeltaTimeAsSeconds())), toolSwitchSpeed * spehs::time::getDeltaTimeAsSeconds()));
			snare->setPosition(spehs::lerp(snare->getPosition(), snare->getPosition() + glm::vec2(0.0f, -applicationData->getWindowHeight()), toolSwitchSpeed * 1.4f * spehs::time::getDeltaTimeAsSeconds()));
		}

		if (inputManager->isKeyPressed(MOUSEBUTTON_LEFT) && !buyButton->getMouseHover())
		{
			if (!tool)
			{
				if (ammunition > 0)
				{
					potatos.push_back(ObjectCreator::createPotato());
					potatos.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/potato.png"));
					potatos.back()->getComponent<spehs::Sprite>()->setColor(glm::vec3(spehs::rng::frandom(0.75f, 1.0f)));
					potatos.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition + (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) / rotationToPosition, 0.0, 0.2f));
					potatos.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 200.0f, spehs::rng::frandom(1.0f, 5.0f)));
					potatos.back()->getComponent<spehs::Transform2D>()->setRotation(spehs::rng::frandom(0.0f, 6.0f));
					potatos.back()->update();

					trumpet->setPosition(trumpet->getPosition().x, -applicationData->getWindowHeightHalf() * 0.3f);
					ammunition--;

					sfxManager.playSound("Sounds/shoot.wav", trumpet->getPosition(), 1.2f);
				}
			}
			else
			{
				if (fishing)
				{
					snare->setPosition(snare->getPosition().x, -applicationData->getWindowHeightHalf() * 0.6f);
					snare->setRotation(-0.25f);

					fishing = false;
					snareLine->setRenderState(false);
					bobber->getComponent<spehs::Sprite>()->setRenderState(false);
					if (catchingFish)
					{
						caught = ObjectCreator::createPotato();//wat ever
						if (!spehs::rng::irandom(0, 3) && !casetteActive)
						{
							caught->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/casette.png"));
							casetteActive = true;
							int speechNum = spehs::rng::irandom(1, 7);
							speech->setSound(spehs::AudioManager::instance->loadWAVE("Sounds/speech" + std::to_string(speechNum) + ".wav"));
							speech->play();
							music->setGain(applicationData->musicVolume * 0.3f);
							sfxManager.playSound("Sounds/fishjump.wav", caught->getComponent<spehs::Transform2D>()->getPosition(), caught->getComponent<CirclePosition>()->getPosition().z, 0.9f, 3.5f);
						}
						else
						{
							float size = spehs::rng::frandom(0.5f, 2.5f);
							caught->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/fish.png"));
							caught->getComponent<spehs::Sprite>()->setSize(size * caught->getComponent<spehs::Sprite>()->getSpritePolygon()->getWidth(), size * caught->getComponent<spehs::Sprite>()->getSpritePolygon()->getHeight());
							food += size;
							sfxManager.playSound("Sounds/fishjump.wav", caught->getComponent<spehs::Transform2D>()->getPosition(), caught->getComponent<CirclePosition>()->getPosition().z, 0.9f, 2.0f * size);
						}
						caught->getComponent<CirclePosition>()->setPosition(bobber->getComponent<CirclePosition>()->getPosition());
						caught->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 1000.0f, -2.0f));
						caught->getComponent<spehs::Transform2D>()->setRotation(PI / 2.0f);
					}
					catchingFish = false;
				}
				else
				{
					snare->setPosition(snare->getPosition().x, -applicationData->getWindowHeightHalf() * 0.3f);
					snare->setRotation(0.25f);
					fishing = true;
					catchingFish = false;
					snareLine->setRenderState(true);
					bobber->getComponent<spehs::Sprite>()->setRenderState(true);
					bobber->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition + (inputManager->getMouseCoords().x - applicationData->getWindowWidthHalf()) / rotationToPosition, 0.0, 0.2f));
					bobber->getComponent<CirclePosition>()->setVelocity(glm::vec3(spehs::rng::frandom(-0.1f, 0.1f), 400.0f, spehs::rng::frandom(3.0f, 6.0f)));
					bobber->getComponent<spehs::Sprite>()->updateVertices();
					fishTimer = spehs::rng::frandom(1.5f, 4.0f);

					sfxManager.playSound("Sounds/siima.wav", snare->getPosition(), 1.2f);
				}
			}
		}

		if (fishing)
		{
			bobber->update();
			snareLine->setPoints(snare->worldVertexArray[1].position, bobber->getComponent<spehs::Sprite>()->getSpritePolygon()->worldVertexArray[1].position);
			bobber->getComponent<CirclePosition>()->setVelocity(bobber->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -3500.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f));
			bobber->getComponent<spehs::Sprite>()->setDepth(-bobber->getComponent<CirclePosition>()->getPosition().z);
			float alpha = std::min(0.0f, std::max(-LOW_WATERS, bobber->getComponent<CirclePosition>()->getPosition().y));
			bobber->getComponent<spehs::Sprite>()->setColorAlpha((LOW_WATERS + alpha) / LOW_WATERS);
			if (bobber->getComponent<CirclePosition>()->getPosition().y < -250.0f)
			{
				bobber->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f));
			}

			static glm::vec3 bobberPlace;
			fishTimer -= spehs::time::getDeltaTimeAsSeconds();
			if (fishTimer <= 0.0f && !catchingFish)
			{
				sfxManager.playSound("Sounds/blub1.wav", bobber->getComponent<spehs::Transform2D>()->getPosition(), bobber->getComponent<CirclePosition>()->getPosition().z, 0.85f, 3.0f);

				catchingFish = true;
				bobberPlace = bobber->getComponent<CirclePosition>()->getPosition() - glm::vec3(0.0f, 200.0f, 0.0f);
			}

			if (catchingFish)
			{
				bobber->getComponent<CirclePosition>()->setPosition(spehs::lerp(bobber->getComponent<CirclePosition>()->getPosition(), bobberPlace, 0.15f));
			}
		}
		if (caught)
		{
			caught->update();
			caught->getComponent<CirclePosition>()->setVelocity(caught->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -1000.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f));

			if (caught->getComponent<CirclePosition>()->getPosition().z < 0.2f)
			{
				sfxManager.playSound("Sounds/bling2.wav", spehs::getActiveBatchManager()->getCamera2D()->position, 1.0f);
				delete caught;
				caught = nullptr;
			}
		}


		if (casetteActive)
		{
			casette->setRenderState(true);

			if (!speech->isPlaying())
			{
				casetteActive = false;
			}
			music->setGain(spehs::lerp(music->getGain(), applicationData->musicVolume * 0.3f, 0.1f));
		}
		else
		{
			casette->setRenderState(false);
			music->setGain(spehs::lerp(music->getGain(), applicationData->musicVolume, 0.1f));
		}


		//THE FINGER
		fingerCooldown -= spehs::time::getDeltaTimeAsSeconds();
		if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
		{
			fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerBase->getPosition().y, (float) -applicationData->getWindowHeightHalf() / 2.0f, 0.2f));
			fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerHighlight->getPosition().y, (float) -applicationData->getWindowHeightHalf() / 2.0f, 0.2f));
			if (fingerCooldown <= 0.0f && food >= FINGER_COST * 1.25f)
			{
				food -= FINGER_COST * 0.25f;
				fingerCooldown = 0.5f;
				for (unsigned i = 0; i < birds.size(); i++)
				{
					food -= FINGER_COST;
					//if (birds[i]->getComponent<CirclePosition>()->getVelocity().z <= -BIRD_MIN_SPEED)
					float angleToBird = birds[i]->getComponent<CirclePosition>()->getPosition().x - spehs::getActiveBatchManager()->getCamera2D()->position.x / rotationToPosition;
					while (abs(angleToBird) > PI)
						angleToBird -= TWO_PI * /*sign*/(abs(angleToBird) / angleToBird);
					if (abs(angleToBird) < PI * 0.1f)
					{//Bird within field of view
						if (birds[i]->getComponent<TargetLock>() == nullptr)
						{
							birds[i]->addComponent<TargetLock>();
							break;
						}
					}
				}
			}
		}
		else
		{
			fingerBase->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerBase->getPosition().y, (float) -applicationData->getWindowHeight(), 0.2f));
			fingerHighlight->setPosition(spehs::getActiveBatchManager()->getCamera2D()->position.x + applicationData->getWindowWidthHalf() / 2.0f, spehs::lerp(fingerHighlight->getPosition().y, (float) -applicationData->getWindowHeight(), 0.2f));
		}
		fingerBase->setColor(glm::vec3(0.2f * std::powf(light, 0.6f), 0.25f * light, 0.22f * light));
		fingerHighlight->setColor(glm::vec3(0.6f + 0.3f * light, std::powf(light, 0.7f), 0.15f + 0.6f * std::powf(light, 0.85f)));

		trumpet->setColor(glm::vec3(0.4f + 0.6f * light, std::powf(light, 0.75f), 0.15f + 0.85f * std::powf(light, 1.5f)));
	}//if !dead

	fishSpawnTimer -= spehs::time::getDeltaTimeAsSeconds();
	if (fishSpawnTimer < 0.0f)
	{
		fishSpawnTimer = FISH_SPAWN_FREQ + spehs::rng::frandom(-0.05f, 0.15f);

		fishes.push_back(ObjectCreator::createFish());
		fishes.back()->getComponent<spehs::Sprite>()->setTexture(textureManager->getTextureData("Textures/fish.png"));
		fishes.back()->getComponent<spehs::Sprite>()->setSize(80.0f, 80.0f);
		fishes.back()->getComponent<CirclePosition>()->setPosition(glm::vec3(spehs::rng::frandom(0, 2 * PI), -200.0f, spehs::rng::frandom(1.0f, 6.0f)));
		fishes.back()->getComponent<CirclePosition>()->setVelocity(glm::vec3(spehs::rng::frandom(-0.1f, 0.1f), 400.0f, spehs::rng::frandom(0.0f, 0.1f)));
		fishes.back()->getComponent<spehs::Transform2D>()->setRotation(spehs::rng::frandom(0.0f, 6.0f));
		fishes.back()->update();

		sfxManager.playSound("Sounds/blub" + std::to_string(spehs::rng::irandom(1, 2)) + ".wav", fishes.back()->getComponent<spehs::Transform2D>()->getPosition(), fishes.back()->getComponent<CirclePosition>()->getPosition().z, 1.15f, 0.1f);
	}

	for (int i = 0; i < fishes.size(); i++)
	{
		fishes[i]->getComponent<CirclePosition>()->setVelocity(fishes[i]->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -600.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f));
		fishes[i]->update();
		fishes[i]->getComponent<spehs::Sprite>()->setDepth(-fishes[i]->getComponent<CirclePosition>()->getPosition().z);
		float alpha = std::min(0.0f, std::max(-LOW_WATERS, fishes[i]->getComponent<CirclePosition>()->getPosition().y));
		fishes[i]->getComponent<spehs::Sprite>()->setColor(glm::vec4(1.0f, 0.25f + 0.75f * light, light, (LOW_WATERS + alpha) / LOW_WATERS));
		fishes[i]->getComponent<spehs::Transform2D>()->setRotation(fishes[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f);

		if (fishes[i]->getComponent<CirclePosition>()->getPosition().y < -LOW_WATERS)
		{
			sfxManager.playSound("Sounds/blub" + std::to_string(spehs::rng::irandom(1, 2)) + ".wav", fishes.back()->getComponent<spehs::Transform2D>()->getPosition(), fishes.back()->getComponent<CirclePosition>()->getPosition().z, 1.4f, 0.35f);

			delete fishes[i];
			fishes[i] = fishes.back();
			fishes.pop_back();
			i--;
		}
	}

	
	for (int i = 0; i < potatos.size(); i++)
	{
		potatos[i]->getComponent<CirclePosition>()->setVelocity(potatos[i]->getComponent<CirclePosition>()->getVelocity() + glm::vec3(0.0f, -40000.0f * spehs::time::getDeltaTimeAsSeconds(), 0.0f) * 0.95f * spehs::time::getDeltaTimeAsSeconds());

		potatos[i]->update();
		potatos[i]->getComponent<spehs::Sprite>()->setDepth(-potatos[i]->getComponent<CirclePosition>()->getPosition().z);
		float alpha = std::min(0.0f, std::max(-LOW_WATERS, potatos[i]->getComponent<CirclePosition>()->getPosition().y));
		potatos[i]->getComponent<spehs::Sprite>()->setColor(glm::vec4(1.0f, std::powf(light, 0.5f), 0.4f + 0.6f * light, (LOW_WATERS + alpha) / LOW_WATERS));
		potatos[i]->getComponent<spehs::Transform2D>()->setRotation(potatos[i]->getComponent<spehs::Transform2D>()->getRotation() + spehs::time::getDeltaTimeAsSeconds() * 2.5f * (i % 2 == 0 ? -1.0f : 1.0f));

		if (potatos[i]->getComponent<CirclePosition>()->getPosition().y < -LOW_WATERS)
		{
			sfxManager.playSound("Sounds/blub" + std::to_string(spehs::rng::irandom(1, 2)) + ".wav", potatos[i]->getComponent<spehs::Transform2D>()->getPosition(), potatos[i]->getComponent<CirclePosition>()->getPosition().z, 0.6f);

			delete potatos[i];
			potatos[i] = potatos.back();
			potatos.pop_back();
			i--;
		}
		else
		{
			static const float X_EPSILON(0.2f);
			static const float Y_EPSILON(2.0f);
			static const float Z_EPSILON(1.0f);
			glm::vec3 potpos, birdpos;
			for (unsigned s = 0; s < birds.size(); s++)
			{
				potpos = potatos[i]->getComponent<CirclePosition>()->getPosition();
				birdpos = birds[s]->getComponent<CirclePosition>()->getPosition();
				if (abs(potpos.x - birdpos.x) < X_EPSILON && abs(potpos.y - birdpos.y) < Y_EPSILON && abs(potpos.z - birdpos.z) < Z_EPSILON)
				{
					hitmarkers.push_back(ObjectCreator::createHitMarker());
					hitmarkers.back()->getComponent<spehs::Transform2D>()->setPosition(birds[s]->getComponent<spehs::Transform2D>()->getPosition());

					sfxManager.playSound("Sounds/bird" + std::to_string(spehs::rng::irandom(1, 3)) + ".wav", birds[s]->getComponent<spehs::Transform2D>()->getPosition(), birds[s]->getComponent<CirclePosition>()->getPosition().z);

					food += BIRD_HEALING;

					delete potatos[i];
					potatos[i] = potatos.back();
					potatos.pop_back();
					i--;

					delete birds[s];
					birds[s] = birds.back();
					birds.pop_back();
					break;
				}
			}
		}
	}

	for (int i = 0; i < hitmarkers.size(); i++)
	{
		hitmarkers[i]->update();

		if (hitmarkers[i]->getComponent<HitMarker>()->done())
		{
			delete hitmarkers[i];
			hitmarkers[i] = hitmarkers.back();
			hitmarkers.pop_back();
			i--;
		}
	}


	for (unsigned i = 0; i < waves.size(); i++)
	{
		waves[i]->update();
		waves[i]->getComponent<spehs::Sprite>()->setDepth(-waves[i]->getComponent<CirclePosition>()->getPosition().z);
	}

	for (unsigned i = 0; i < clouds.size(); i++)
	{
		clouds[i]->getComponent<Cloud>()->baseSprite->setDepth(-clouds[i]->getComponent<CirclePosition>()->getPosition().z);
		clouds[i]->update();
	}


	for (int i = 0; i < birds.size(); i++)
	{
		if (sin(sunPosition + TWO_PI / DAY_CYCLE_SECONDS * 2.0f/*Seconds spawning begins/ends in advance*/) < 0.2f)
		{
			birds[i]->getComponent<CirclePosition>()->setVelocity(glm::vec3(0.0f, 0.0f, -spehs::rng::frandom(BIRD_MIN_SPEED, BIRD_MAX_SPEED)));
		}
		birds[i]->update();
		birds[i]->getComponent<spehs::Sprite>()->setDepth(-birds[i]->getComponent<CirclePosition>()->getPosition().z);

		if (birds[i]->getComponent<CirclePosition>()->getPosition().z < 0.1f)
		{
			food -= BIRD_DAMAGE * day;

			if (!dead)
			{
				sfxManager.playSound("Sounds/damage" + std::to_string(spehs::rng::irandom(1, 3)) + ".wav", spehs::getActiveBatchManager()->getCamera2D()->position, 1.0f);
			}

			delete birds[i];
			birds[i] = birds.back();
			birds.pop_back();
			i--;
		}
	}
}

void Environment::buyPotato()
{
	static const float minFoodToBuyPotatoes = 1.5f;
	if (food > minFoodToBuyPotatoes)
	{
		food--;
		ammunition += FISH_TO_POTATO;

		sfxManager.playSound("Sounds/bling1.wav", spehs::getActiveBatchManager()->getCamera2D()->position, 1.0f);
	}
}

void Environment::nextMusic()
{
	switch (musicName)
	{
	case MusicName::Day1: musicName = MusicName::Night1; break;
	case MusicName::Day2: musicName = MusicName::Night2; break;
	case MusicName::Night1: musicName = MusicName::Day2; break;
	case MusicName::Night2: musicName = MusicName::Day1; break;
	}
	music->setSound("Music/" + musicNametoString(musicName) + ".wav");
	music->play();
}

std::string Environment::musicNametoString(MusicName _musicName)
{
	switch (_musicName)
	{
	default: return "Day1";
	case MusicName::Day2: return "Day2";
	case MusicName::Night1: return "Night1";
	case MusicName::Night2: return "Night2";
	}
}
