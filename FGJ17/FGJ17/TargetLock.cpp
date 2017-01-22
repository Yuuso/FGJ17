#include <algorithm>
#include <SpehsEngine/Line.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/Geometry.h>
#include <SpehsEngine/GameObject.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/StringOperations.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Sprite.h>
#include <SpehsEngine/Text.h>
#include "TargetLock.h"
#include "CirclePosition.h"
#include "Environment.h"
#include "GameState.h"
extern const float rotationToPosition;
#define CROSSHAIR_RADIUS 10.0f


TargetLock::TargetLock() : animationState(0.0f)
{
	initialize();
}
TargetLock::TargetLock(spehs::GameObject& _owner) : spehs::Component(_owner), animationState(0.0f)
{
	initialize();
}
TargetLock::~TargetLock()
{
	for (unsigned i = 0; i < 12; i++)
		lines[i]->destroy();
	text->destroy();
}
void TargetLock::initialize()
{
	if (lines[0])
		return;
	for (unsigned i = 0; i < 12; i++)
	{
		lines[i] = spehs::Line::create(30000);
		lines[i]->setCameraMatrixState(false);
		lines[i]->setLineWidth(i % 3 == 0 ? 1.5f : 5.0f);
	}
	glm::vec3 vel = ownerObject->getComponent<CirclePosition>()->getVelocity();
	vel.z *= 0.7f;
	ownerObject->getComponent<CirclePosition>()->setVelocity(vel);

	text = spehs::Text::create(30000);
	text->setFont("Fonts/Anonymous.ttf", 12);
	text->setCameraMatrixState(false);
}
void TargetLock::update()
{
	static const float animationStateExponent = 2.0f;
	animationState += spehs::time::getDeltaTimeAsSeconds() * 0.9f;
	if (animationState > 1.0f)
		animationState = 1.0f;

	const glm::vec2 windowCenter = glm::vec2(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf());
	const float radToWindowCorner = spehs::magnitude(windowCenter);
	const float rad = std::max(ownerObject->getComponent<spehs::Sprite>()->getSpritePolygon()->getHeight(), ownerObject->getComponent<spehs::Sprite>()->getSpritePolygon()->getWidth());
	const glm::vec2 pos(applicationData->getWindowWidthHalf() + ownerObject->getComponent<CirclePosition>()->getPosition().x * rotationToPosition - GameState::instance->getCamera().position.x, applicationData->getWindowHeightHalf() + ownerObject->getComponent<spehs::Sprite>()->getSpritePolygon()->getPosition().y);

	if (pos.x < 0.0f || pos.x > applicationData->getWindowWidth())
	{
		for (unsigned i = 0; i < 12; i++)
			lines[i]->setRenderState(false);
		text->setRenderState(false);
		return;
	}
	const float distanceFactor = std::powf(std::min(1.0f, abs(ownerObject->getComponent<CirclePosition>()->getPosition().z / (0.5f * MAX_SPAWN_HEIGHT))), 0.15f);
	for (unsigned i = 0; i < 12; i++)
	{
		lines[i]->setRenderState(true);
		lines[i]->setColor(glm::vec4(1.0f, distanceFactor, distanceFactor, i % 3 == 0 ? 0.15f : 0.4f));
	}

	glm::vec2 corner = glm::vec2(applicationData->getWindowWidth(), applicationData->getWindowHeight());
	glm::vec2 point = corner + std::powf(animationState, animationStateExponent) * (pos - corner);
	lines[0]->setPoints(corner, point);// corner + animationState * (pos - corner));
	lines[1]->setPoints(point, point + glm::vec2(-CROSSHAIR_RADIUS, 0.0f));
	lines[2]->setPoints(point, point + glm::vec2(0.0f, -CROSSHAIR_RADIUS));

	corner = glm::vec2(0.0f, applicationData->getWindowHeight());
	point = corner + std::powf(animationState, animationStateExponent) * (pos - corner);
	lines[3]->setPoints(corner, point);// corner + animationState * (pos - corner));
	lines[4]->setPoints(point, point + glm::vec2(CROSSHAIR_RADIUS, 0.0f));
	lines[5]->setPoints(point, point + glm::vec2(0.0f, -CROSSHAIR_RADIUS));

	corner = glm::vec2(0.0f, 0.0f);
	point = corner + std::powf(animationState, animationStateExponent) * (pos - corner);
	lines[6]->setPoints(corner, point);// corner + animationState * (pos - corner));
	lines[7]->setPoints(point, point + glm::vec2(0.0f, CROSSHAIR_RADIUS));
	lines[8]->setPoints(point, point + glm::vec2(CROSSHAIR_RADIUS, 0.0f));

	corner = glm::vec2(applicationData->getWindowWidth(), 0.0f);
	point = corner + std::powf(animationState, animationStateExponent) * (pos - corner);
	lines[9]->setPoints(corner, point);// corner + animationState * (pos - corner));
	lines[10]->setPoints(point, point + glm::vec2(-CROSSHAIR_RADIUS, 0.0f));
	lines[11]->setPoints(point, point + glm::vec2(0.0f, CROSSHAIR_RADIUS));

	text->setRenderState(true);
	text->setString(spehs::toString(ownerObject->getComponent<CirclePosition>()->getPosition().z * 20.0f, 2) + "m");
	text->setPosition(pos + glm::vec2(text->getTextWidth() * -0.5f, text->getTextHeight() * -0.5f - CROSSHAIR_RADIUS));
	text->setAlpha(0.8f);
}