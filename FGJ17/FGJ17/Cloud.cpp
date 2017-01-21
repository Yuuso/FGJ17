#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/Sprite.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Transform2D.h>
#include <SpehsEngine/GameObject.h>
#include "Cloud.h"


glm::vec3 Cloud::baseColor = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 Cloud::highlightColor = glm::vec3(1.0f, 1.0f, 1.0f);
Cloud::Cloud() : highlightPolygon(nullptr)
{
}
Cloud::Cloud(spehs::GameObject& _owner) : spehs::Component(_owner), highlightPolygon(nullptr)
{

}
Cloud::~Cloud()
{
	if (highlightPolygon)
		highlightPolygon->destroy();
}
void Cloud::update()
{
	baseSprite->setColor(baseColor);

	if (!highlightPolygon)
		createHighlight();
	highlightPolygon->setColor(highlightColor);
	highlightPolygon->setPosition(baseSprite->getSpritePolygon()->getPosition());
	highlightPolygon->setPlaneDepth(baseSprite->getSpritePolygon()->getPlaneDepth() + 1);
	highlightPolygon->setScale(ownerObject->getComponent<spehs::Transform2D>()->getScale());
}
void Cloud::createHighlight(const int _cloudIndex)
{
	cloudIndex = _cloudIndex;
}
void Cloud::createHighlight()
{
	highlightPolygon = spehs::Polygon::create(4, baseSprite->getSpritePolygon()->getPlaneDepth() + 1, baseSprite->getSpritePolygon()->getWidth(), baseSprite->getSpritePolygon()->getHeight());
	highlightPolygon->setTexture(textureManager->getTextureData("Textures/cloud_highlight" + std::to_string(cloudIndex) + ".png", spehs::TextureFiltering::Nearest, spehs::TextureFiltering::Nearest));
}