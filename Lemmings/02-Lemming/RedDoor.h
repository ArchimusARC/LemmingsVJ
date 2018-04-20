#ifndef _REDDOOR_INCLUDE
#define _REDDOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "../irrKlang-1.6.0/include/irrKlang.h"


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
	void displacement(int displ2);

private:

	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	irrklang::ISoundEngine* engine;
	int opener;
};


#endif // _REDDOOR_INCLUDE
