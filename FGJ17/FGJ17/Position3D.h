
#pragma once

#include <SpehsEngine/Component.h>

#include <glm/vec3.hpp>


class PseudoCamera;
class Position3D : public spehs::Component
{
public:
	Position3D();
	~Position3D();

	void update() override;

	void setPosition(const glm::vec3& _pos);
	void setVelocity(const glm::vec3& _vel);
	void setVelocityTowards(const glm::vec3& _vel);

	void setPseudoCamera(PseudoCamera* _camera);

private:
	glm::vec3 position;
	glm::vec3 velocity;
	float rotation;
	PseudoCamera* camera;
};

