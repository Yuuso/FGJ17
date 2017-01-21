
#pragma once
#define DAY_CYCLE_SECONDS 45.0f
#define MAX_SPAWN_DISTANCE 100.0f

#include <vector>
#include <glm/vec3.hpp>


namespace spehs{ class GameObject; class Polygon; }
class Environment
{
	static glm::vec3 seaColor;
	static float sunPosition;
	static float light;
	static int day;
public:
	Environment();
	~Environment();

	void update();

private:
	std::vector<spehs::GameObject*> birds;
	std::vector<spehs::GameObject*> potatos;
	std::vector<spehs::GameObject*> waves;
	std::vector<spehs::GameObject*> clouds;
	std::vector<spehs::GameObject*> fishes;
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
	spehs::Polygon* fingerBase;
	spehs::Polygon* fingerHighlight;
	float fingerCooldown;
};

