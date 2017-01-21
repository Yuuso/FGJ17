
#pragma once

#include <SpehsEngine/Component.h>

#include <glm/vec3.hpp>


namespace spehs{ class GameObject; }
class WaveMotion : public spehs::Component
{
public:
	WaveMotion();
	WaveMotion(spehs::GameObject& _owner);
	~WaveMotion() override;

	void update() override;

	void setValue(const float _value);

private:
	float value;
	glm::vec3 pos;
};

