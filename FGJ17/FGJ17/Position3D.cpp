
#include "Position3D.h"

#include "PseudoCamera.h"

#include <SpehsEngine/Time.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/GameObject.h>

#include <glm/geometric.hpp>


Position3D::Position3D()
{
}
Position3D::~Position3D()
{
}

void Position3D::update()
{
	position += velocity * spehs::time::getDeltaTimeAsSeconds();

	spehs::Transform2D* transform = ownerObject->getComponent<spehs::Transform2D>();
	assert(transform);

	float d = glm::length(glm::cross(position - glm::vec3(0.0f), position - camera->getOrientation()) / (camera->getOrientation() - glm::vec3(0.0f)));

	transform->setPosition();
}

void Position3D::setPosition(const glm::vec3& _pos)
{
	position = _pos;
}
void Position3D::setVelocity(const glm::vec3& _vel)
{
	velocity = _vel;
}
void Position3D::setVelocityTowards(const glm::vec3& _vel)
{
	velocity = _vel - position;
}

void Position3D::setPseudoCamera(PseudoCamera* _camera)
{
	camera = _camera;
}