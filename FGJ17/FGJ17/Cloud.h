#include <SpehsEngine/Component.h>
#define NUM_CLOUD_VARIATIONS 3
namespace spehs
{
	class Sprite;
	class Polygon;
}

class Cloud : public spehs::Component
{
public:
	static glm::vec3 baseColor;
	static glm::vec3 highlightColor;
public:
	Cloud();
	Cloud(spehs::GameObject& _owner);
	~Cloud() override;
	void createHighlight(const int cloudIndex);
	
	void update() override;

	spehs::Sprite* baseSprite;
private:
	void createHighlight();
	int cloudIndex;
	spehs::Polygon* highlightPolygon;
};