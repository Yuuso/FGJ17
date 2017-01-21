
#pragma once

#include <vector>


namespace spehs{ class GameObject; class Polygon; }
class Environment
{
public:
	Environment();
	~Environment();

	void update();

private:
	std::vector<spehs::GameObject*> birds;
	std::vector<spehs::GameObject*> potatos;
	std::vector<spehs::GameObject*> waves;
	std::vector<spehs::GameObject*> fishes;
	spehs::Polygon* finger;
	spehs::Polygon* background;
	spehs::Polygon* background2;
	spehs::Polygon* background3;
	spehs::Polygon* trumpet;
};

