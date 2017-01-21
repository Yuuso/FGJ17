
#pragma once

#include <SpehsEngine/Component.h>

#include <glm/vec3.hpp>


namespace spehs{ class GameObject; }
class CirclePosition : public spehs::Component
{
public:
	CirclePosition();
	CirclePosition(spehs::GameObject& _owner);
	~CirclePosition();

	void update() override;

	void setPosition(const glm::vec3& _pos);
	void setVelocity(const glm::vec3& _pos);

	glm::vec3 getPosition() const { return position; }

private:
	glm::vec3 position; //(circle x position, height, distance)
	glm::vec3 velocity;
};

