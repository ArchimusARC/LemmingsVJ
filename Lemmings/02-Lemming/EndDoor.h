#ifndef _ENDDOOR_INCLUDE
#define _ENDDOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


//End door, goal of the game


class EndDoor
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int mapDispl);
	void update(int deltaTime);
	void render();

	void setMapMask(VariableTexture *mapMask);
	glm::ivec2 position();

private:

	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	int displ;
};


#endif // _ENDDOOR_INCLUDE
