
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
	std::vector<spehs::GameObject*> objects;
	spehs::Polygon* background;
	spehs::Polygon* background2;
	spehs::Polygon* background3;
};

