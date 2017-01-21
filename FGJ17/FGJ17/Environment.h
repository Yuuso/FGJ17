
#pragma once

#include <vector>


namespace spehs{ class GameObject; class Polygon; }
class Environment
{
	static float sunPosition;
public:
	Environment();
	~Environment();

	void update();

private:
	std::vector<spehs::GameObject*> birds;
	std::vector<spehs::GameObject*> potatos;
	std::vector<spehs::GameObject*> waves;
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
};

