#ifndef _REDDOOR_INCLUDE
#define _REDDOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class RedDoor
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int firstFrame);
	void update(int deltaTime);
	void render();

	void setMapMask(VariableTexture *mapMask);
	bool opened(int currentFrame);

private:

	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	int opener;
	int firstFrame;
};


#endif // _REDDOOR_INCLUDE
