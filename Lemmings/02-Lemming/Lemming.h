#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "../irrKlang-1.6.0/include/irrKlang.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int mapDispl);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);
	glm::ivec2 position();
	void give(int alt);
	bool inTheBox(int x, int y);
	int report();
	void displacement(int displ2);
	void setDisplacement(int displacement);
	
private:
	float LemmingAnimations = 8.0f;//Modificar cada cop que s'afegeixi una animació
	int fallDistance;
	int collisionFloor(int maxFall);
	bool collision();
	bool continue_bashing();
	bool notColliding();
	bool notABlocker();
	bool stairCollision();
	bool iSync();
	bool iDoRender();
	bool iDoNotUpdate();
	bool buildFinished();
	
	void setAnimations(ShaderProgram &shaderProgram);
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, DEAD, 
		DIGGING, BLOCKING, BASHING_LEFT, BASHING_RIGHT, CLIMBING_LEFT, CLIMBING_RIGHT, BUILDING_LEFT, 
		BUILDING_RIGHT, FLOATING_LEFT, FLOATING_RIGHT, VICTORIOUS, EXPLODING
	};
	enum AltState //estats seguents que no podem controlar amb la maquina d'estats
	{
		NONE, DIGGER, CLIMBER, BASHER, PARACHUTE, BLOCKER, BUILDER, WINNER, EXPLODE	
	};
	int displ, build;
	LemmingState state;
	AltState given;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	irrklang::ISoundEngine* engine;
};


#endif // _LEMMING_INCLUDE


