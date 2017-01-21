
#pragma once


namespace spehs{ class GameObject; }
class ObjectCreator
{
public:
	static spehs::GameObject* createBird();
	static spehs::GameObject* createPotato();
	static spehs::GameObject* createWaves();
	static spehs::GameObject* createFish();
	static spehs::GameObject* createCloud();
};

