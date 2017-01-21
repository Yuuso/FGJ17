#pragma once
#include <SpehsEngine/Component.h>
#define PI 3.14159265358f
namespace spehs
{
	class Polygon;
	class Line;
}

class TargetLock : public spehs::Component
{
public:
	TargetLock();
	TargetLock(spehs::GameObject& _owner);
	~TargetLock() override;

	void update() override;
private:
	void initialize();
	spehs::Line* lines[12];
	float animationState;
};