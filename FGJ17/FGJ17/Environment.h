
#pragma once

#include <vector>


namespace spehs{ class GameObject; }
class Environment
{
public:
	Environment();
	~Environment();

	void update();

private:
	std::vector<spehs::GameObject*> objects;
};

