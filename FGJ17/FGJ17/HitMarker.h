
#pragma once

#include <SpehsEngine/Component.h>


#define HITMARK_TIMER 0.4f


namespace spehs{ class GameObject; }
class HitMarker : public spehs::Component
{
public:
	HitMarker();
	HitMarker(spehs::GameObject& _owner);
	~HitMarker() override;

	void update() override;
	bool done() const { return pleaseDelete; }

private:
	const float sizeSpeed = 1.5f;
	const float rotSpeed = 5.0f;
	float timer = HITMARK_TIMER;
	bool pleaseDelete = false;
};

