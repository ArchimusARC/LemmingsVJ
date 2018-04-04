#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);
	
private:
	float LemmingAnimations = 8.0f;//Modificar cada cop que s'afegeixi una animació
	int fallDistance;
	int collisionFloor(int maxFall);
	bool collision();
	bool continue_bashing();
	bool notColliding();
	bool notABlocker();
	bool stairCollision();
	void setAnimations(ShaderProgram &shaderProgram);
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, 
		DIGGING, BLOCKING, BASHING_LEFT, BASHING_RIGHT, CLIMBING_LEFT, CLIMBING_RIGHT, BUILDING_LEFT, 
		BUILDING_RIGHT, FLOATING_LEFT, FLOATING_RIGHT
	};
	enum AltState //estats seguents que no podem controlar amb la maquina d'estats
	{
		NONE, DIGGER, CLIMBER, BASHER, PARACHUTE, BLOCKER, BUILDER
	};

	LemmingState state;
	AltState given;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
};


#endif // _LEMMING_INCLUDE


