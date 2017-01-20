
#pragma once

#include "PseudoCamera.h"

#include <vector>


namespace spehs{ class GameObject; }
class Environment
{
public:
	Environment();
	~Environment();

	void update();

private:
	PseudoCamera camera;
	std::vector<spehs::GameObject*> objects;
};

