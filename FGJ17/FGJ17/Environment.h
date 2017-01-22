
#pragma once

#include "SFXManager.h"

#include <vector>
#include <glm/vec3.hpp>


#define DAY_CYCLE_SECONDS 120.0f
#define MAX_SPAWN_HEIGHT 100.0f
#define FINGER_COST 0.05f
#define BIRD_MIN_SPEED 0.5
#define BIRD_MAX_SPEED 1.5f


namespace spehs{ namespace audio{ class SoundSource; } class GameObject; class Polygon; class GUIRectangle; class Line; }
class Environment
{
public:
	Environment();
	~Environment();

	void update();

	void buyPotato();

	bool getDead(){ return dead; }
	int score;

private:
	std::vector<spehs::GameObject*> birds;
	std::vector<spehs::GameObject*> potatos;
	std::vector<spehs::GameObject*> waves;
	std::vector<spehs::GameObject*> clouds;
	std::vector<spehs::GameObject*> fishes;
	std::vector<spehs::GameObject*> hitmarkers;
	struct Background
	{
		Background(const float x);
		~Background();
		void update();
		spehs::Polygon* sea;
		spehs::Polygon* seaShade;
		spehs::Polygon* sky;
		spehs::Polygon* skyShade;
		spehs::Polygon* stars;
	};
	Background* backgroundLeft;
	Background* backgroundCenter;
	Background* backgroundRight;
	spehs::Polygon* trumpet;
	spehs::Polygon* snare;
	spehs::Line* snareLine;
	spehs::GameObject* bobber;
	spehs::GameObject* caught;
	spehs::Polygon* fingerBase;
	spehs::Polygon* fingerHighlight;
	float fingerCooldown;

	float fishSpawnTimer = 0.0f;

	bool dead = false;

	bool tool = true; //true = snare, false = trumpet
	bool fishing = false;
	bool catchingFish = false;
	float fishTimer;

	float food = 10.0f;
	static const unsigned int FISH_TO_POTATO = 10;
	int ammunition = 30;

	spehs::Polygon* foodBar;
	spehs::Polygon* foodLogo;
	spehs::Polygon* ammunitionBar;
	spehs::Polygon* ammunitionLogo;
	spehs::GUIRectangle* buyButton;

	bool casetteActive = false;
	spehs::Polygon* casette;
	spehs::audio::SoundSource* speech;

	spehs::audio::SoundSource* music;
	spehs::audio::SoundSource* waveSounds;

	SFXManager sfxManager;
};

