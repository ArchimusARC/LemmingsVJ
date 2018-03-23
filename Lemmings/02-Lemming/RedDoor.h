#ifndef _REDDOOR_INCLUDE
#define _REDDOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class RedDoor
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setMapMask(VariableTexture *mapMask);
	bool opened();

private:

	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	int opener;
};


#endif // _REDDOOR_INCLUDE
