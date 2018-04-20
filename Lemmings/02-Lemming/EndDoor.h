#ifndef _ENDDOOR_INCLUDE
#define _ENDDOOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "../irrKlang-1.6.0/include/irrKlang.h"


//End door, goal of the game


class EndDoor
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int mapDispl);
	void update(int deltaTime);
	void render();

	void setMapMask(VariableTexture *mapMask);
	glm::ivec2 position();
	void displacement(int displ2);

private:

	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	int displ;
	irrklang::ISoundEngine* engine;
};


#endif // _ENDDOOR_INCLUDE
