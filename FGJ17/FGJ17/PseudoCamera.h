
#pragma once

#include <glm/vec3.hpp>


class PseudoCamera
{
public:
	PseudoCamera();
	~PseudoCamera();

	void yaw(const float _degrees); //x
	void pitch(const float _degrees); //y

	glm::vec3 getOrientation() const { return orientation; }
	float getFOV() const { return FOV; }

private:
	const glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 1.0f); //orientation vector
	float FOV = 0.25f; //Pseudo fov used as a factor for dotproducting
};

