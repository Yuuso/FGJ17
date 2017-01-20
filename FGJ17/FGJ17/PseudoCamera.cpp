
#include "PseudoCamera.h"

#include <algorithm>


const float MAX_PITCH(0.5f);


PseudoCamera::PseudoCamera()
{
}
PseudoCamera::~PseudoCamera()
{
}


void PseudoCamera::yaw(const float _degrees)
{
	orientation.x = sin(360.0f / _degrees);
	orientation.z = cos(360.0f / _degrees);
}
void PseudoCamera::pitch(const float _degrees)
{
	orientation.y = std::max(-MAX_PITCH, std::min(MAX_PITCH, 360.0f / _degrees));
}
