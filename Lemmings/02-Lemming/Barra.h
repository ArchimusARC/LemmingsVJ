#ifndef _BARRA_INCLUDE
#define _BARRA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Barra
{

public:

	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int displacement);
	void render();
	int checkState(int x, int y);
	void setDispl(int mapDispl);
	void setMapMask(VariableTexture *mapMask);

private:

private:
	enum BarraState
	{
		NONE, CLIMBER, PARACHUTE, BLOCKER, BUILDER, BASHER, DIGGER, EXPLODER, ACCELERATE, PAUSE, EXPLOSION
	};

	BarraState state, lastState;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	glm::vec2 position;
	int displ;
};


#endif // _BARRA_INCLUDE
