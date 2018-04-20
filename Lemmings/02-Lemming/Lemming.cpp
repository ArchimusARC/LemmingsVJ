#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	 WALKING_RIGHT, WALKING_LEFT, FALLING_RIGHT, FALLING_LEFT,
	UMBRELLA_RIGHT, UMBRELLA_LEFT, PARACHUTE_RIGHT, PARACHUTE_LEFT, ENDWALK, DIG, STOPPER,
	CLIMBING_RIGHT, CLIMBING_LEFT, BUILDING_RIGHT, BUILDING_LEFT, BASHING1_RIGHT, BASHING2_RIGHT, BASHING1_LEFT, BASHING2_LEFT,
	DEATH, IAMABOUTTOEXPLODE, EXPLODE

};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int mapDispl)
{
	build = 0;
	state = FALLING_RIGHT_STATE;
	given = NONE;
	engine = irrklang::createIrrKlangDevice();
	setAnimations(shaderProgram);
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
	displ = mapDispl;
}

void Lemming::update(int deltaTime)
{
	int fall;
	if (iDoNotUpdate()) return;
	if (sprite->update(deltaTime) == 0) return;
	if (iSync()) return;
	if (given == WINNER && state != VICTORIOUS) {
		engine->play2D("coin.wav", false);
		sprite->changeAnimation(ENDWALK);
		state = VICTORIOUS;
	}
	else if (given == EXPLODE) {
		state = EXPLODING;
		engine->play2D("explode.wav", false);
		//sprite->changeAnimation(EXPLODE);
	}
	else if (given == BLOCKER && (state != FALLING_LEFT_STATE || state != FALLING_RIGHT_STATE) && state != BLOCKING) {
		sprite->changeAnimation(STOPPER);
		state = BLOCKING;
		
	}
	else if (given == DIGGER && (state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE)) {
		sprite->changeAnimation(DIG);
		state = DIGGING;
		
	}
	else if (given == BUILDER && state == WALKING_LEFT_STATE) {
		sprite->changeAnimation(BUILDING_LEFT);
		state = BUILDING_LEFT;
	}
	else if (given == BUILDER && state == WALKING_RIGHT_STATE) {
		sprite->changeAnimation(BUILDING_RIGHT);
		state = BUILDING_RIGHT;
	}
	bool col;
	glm::ivec2 actualPos;
	switch (state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if (fall > 1) {
			sprite->position() += glm::vec2(0, fall);
			fallDistance += fall;
			if (given == PARACHUTE && sprite->animation() != PARACHUTE_LEFT) {
				engine->play2D("pop.wav", false);
				sprite->changeAnimation(PARACHUTE_LEFT);
			}	
			actualPos = sprite->position();
			if (actualPos.y + fall >= 170) {
				engine->play2D("roblox_death.wav", false);
				sprite->changeAnimation(DEATH);
				state = DEAD;
				
			}
		}
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			if (fallDistance + fall > 50 && given != PARACHUTE) {
				sprite->changeAnimation(DEATH);
				engine->play2D("roblox_death.wav", false);
				state = DEAD;
				
			}
			else given = NONE;
		}
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if (fall > 1) {
			sprite->position() += glm::vec2(0, fall);
			fallDistance += fall;
			if (given == PARACHUTE && sprite->animation() != PARACHUTE_RIGHT) {
				engine->play2D("pop.wav", false);
				sprite->changeAnimation(PARACHUTE_RIGHT);
			}
			actualPos = sprite->position();
			if (actualPos.y + fall >= 170) {
				sprite->changeAnimation(DEATH);
				engine->play2D("roblox_death.wav", false);
				state = DEAD;
				
			}
		}
		else {
			if (fallDistance + fall > 50 && given != PARACHUTE) {
				sprite->changeAnimation(DEATH);
				engine->play2D("roblox_death.wav", false);
				state = DEAD;
				
			}
			else {
				given = NONE;
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		col = collision();
		if (col && given == CLIMBER)
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(CLIMBING_LEFT);
			state = CLIMBING_LEFT;
		}
		else if (col && given == BASHER) {
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(BASHING_LEFT);
			state = BASHING_LEFT;
		}
		else if (col) {
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
				fallDistance = fall;
			}
		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		col = collision();
		if (col && given == CLIMBER)
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(CLIMBING_RIGHT);
			state = CLIMBING_RIGHT;
		}
		else if (col && given == BASHER) {
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(BASHING_RIGHT);
			state = BASHING_RIGHT;
		}
		else if (col) {
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
				fallDistance = fall;
			}
		}
		break;
	case DEAD:
		//sprite->setPosition(glm::vec2(32, 170));
		break;
	case DIGGING:
		//if (sprite->currentFrame() == 7) sprite->changeAnimation(DIG);
		actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
		actualPos += glm::ivec2(7, 15);//sprite displacement
		col = notABlocker();
		if (col) {
			for (int i = 0; i < 5; ++i) {
				mask->setPixel(actualPos.x + i - 2, actualPos.y + 1, 0);
				engine->play2D("dig.wav", false);
			}
		}
		fall = collisionFloor(3);
		if (fall < 3)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(FALLING_RIGHT);
			state = FALLING_RIGHT_STATE;
			fallDistance = fall;
			given = NONE;
		}
		break;
	case CLIMBING_LEFT:
		if (notColliding()) {
			sprite->position() += glm::vec2(0, -1);
			actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
			actualPos += glm::ivec2(7, 15);//sprite displacement
			if (mask->pixel(actualPos.x - 1, actualPos.y) == 0) state = WALKING_LEFT_STATE;
		}
		break;
	case CLIMBING_RIGHT:
		if (notColliding()) {
			sprite->position() += glm::vec2(0, -1);
			actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
			actualPos += glm::ivec2(7, 15);//sprite displacement
			if (mask->pixel(actualPos.x + 2, actualPos.y) == 0) state = WALKING_RIGHT_STATE;
		}
		break;
	case BASHING_LEFT:
		if (continue_bashing() && notABlocker()) {
			actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
			actualPos += glm::ivec2(7, 15);//sprite displacement
			for (int i = 0; i < 10; ++i) {
				engine->play2D("dig.wav", false);
				mask->setPixel(actualPos.x - 1, actualPos.y - i, 0);
			}
		}
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			given = NONE;
		}
		sprite->position() += glm::vec2(-1, -1);
		fall = collisionFloor(3);
		if (fall < 3)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(FALLING_RIGHT);
			state = FALLING_RIGHT_STATE;
			fallDistance = fall;
		}
		break;
	case BASHING_RIGHT:
		if (continue_bashing() && notABlocker()) {
			actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
			actualPos += glm::ivec2(7, 15);//sprite displacement
			for (int i = 0; i < 10; ++i) {
				engine->play2D("dig.wav", false);
				mask->setPixel(actualPos.x + 2, actualPos.y - i, 0);
			}
		}
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			given = NONE;
		}
		sprite->position() += glm::vec2(1, -1);
		fall = collisionFloor(3);
		if (fall < 3)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(FALLING_LEFT);
			state = FALLING_LEFT_STATE;
			fallDistance = fall;
		}
		break;
	case BLOCKING:
		actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
		actualPos += glm::ivec2(7, 15);//sprite displacement
		
		for (int i = -4; i < 6; ++i) {
			mask->setPixel(actualPos.x + i, actualPos.y, 200);
			mask->setPixel(actualPos.x + i, actualPos.y - 9, 200);
		}
		for (int i = 0; i < 10; ++i) {
			mask->setPixel(actualPos.x - 4, actualPos.y - i, 200);
			mask->setPixel(actualPos.x + 6, actualPos.y - i, 200);
		}
		break;
	case BUILDING_LEFT:
		actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
		actualPos += glm::ivec2(7, 15);//sprite displacement
		if (!stairCollision() && !buildFinished()) {
			for (int i = 1; i < 7; ++i) {
				engine->play2D("build.wav", false);
				mask->setPixel(actualPos.x - i, actualPos.y, 255);
			}
			sprite->position() += glm::vec2(-2, -1);
		}
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			given = NONE;
		}
		break;
	case BUILDING_RIGHT:
		actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
		actualPos += glm::ivec2(7, 15);//sprite displacement
		if (!stairCollision() && !buildFinished()) {
			for (int i = 2; i < 8; ++i) {
				engine->play2D("build.wav", false);
				mask->setPixel(actualPos.x + i, actualPos.y, 255);
			}
			sprite->position() += glm::vec2(2, -1);
		}
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			given = NONE;
		}
		break;
	case EXPLODING:
		/*if (sprite->currentFrame() == 15 && sprite->animation() == IAMABOUTTOEXPLODE) sprite->changeAnimation(EXPLODE);
		else if (sprite->animation() == EXPLODE && sprite->currentFrame() == 15) {*/
			state = DEAD;
			actualPos = sprite->position();//position + map displacement
			actualPos += glm::ivec2(7, 12);//sprite displacement
			actualPos.x += displ;

			for (int y = actualPos.y- 6; y <= actualPos.y + 6; y++) {
				for (int x = actualPos.x - 6; x <= actualPos.x + 6; x++) {
					if (sqrt(pow(x - actualPos.x, 2) + pow(y - actualPos.y, 2)) < 6) mask->setPixel(x, y, 0);
				}
			}
		//}
		break;
	}
}

void Lemming::render()
{
	if (iDoRender())sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(displ, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) ==0) && (mask->pixel(posBase.x+1, posBase.y+fall) ==0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(displ, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::continue_bashing() {
	glm::ivec2 posBase = sprite->position() + glm::vec2(displ, 0); // Add the map displacement
	posBase += glm::ivec2(7, 15);
	bool right = state == BASHING_RIGHT;
	for (int i = 0; i < 10; ++i) {
		if (mask->pixel(posBase.x -1 + 3 * right, posBase.y - i) == 255)
			return true;
	}
	return false;
}

bool Lemming::notColliding() {
	glm::ivec2 posCap = sprite->position() + glm::vec2(displ, 0); // Add the map displacement
	posCap += glm::ivec2(7, 5);
	if (mask->pixel(posCap.x, posCap.y) < 51 && mask->pixel(posCap.x + 1, posCap.y) == 0)return true;
	else return false;
}

bool Lemming::notABlocker() {
	glm::ivec2 actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
	actualPos += glm::ivec2(7, 15);//sprite displacement
	if (state == DIGGING) {
		for (int i = 0; i < 5; ++i) {
			if (mask->pixel(actualPos.x + i - 2, actualPos.y + 1) == 200) return false;
		}
		return true;
	}
	else {
		bool right = state == BASHING_RIGHT;
		for (int i = 0; i < 10; ++i) {
			if (mask->pixel(actualPos.x - 1 + 3 * right, actualPos.y - i) == 200)
				return false;
		}
		return true;
	}
}

bool Lemming::stairCollision() {
	glm::ivec2 actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
	actualPos += glm::ivec2(7, 15);//sprite displacement
	if (state == BUILDING_RIGHT) {
		for (int i = 2; i <4; ++i) {
			if (mask->pixel(actualPos.x + i, actualPos.y - 1) == 255 || mask->pixel(actualPos.x + i, actualPos.y - 2) == 255) return true;
		}
		return false;
	}
	else {
		for (int i = 1; i < 3; ++i) {
			if (mask->pixel(actualPos.x - i, actualPos.y - 1) == 255 || mask->pixel(actualPos.x - i, actualPos.y - 2) == 255) return true;
		}
		return false;
	}
}

bool Lemming::iSync() {
	switch (state) {
	case DIGGING:
		if ((sprite->currentFrame() == 7) || (sprite->currentFrame() == 15)) return false;
		else return true;
		break;
	case BASHING_LEFT:
		if ((sprite->currentFrame() == 7) || (sprite->currentFrame() == 15)) return false;
		else return true;
		break;
	case BASHING_RIGHT:
		if ((sprite->currentFrame() == 7) || (sprite->currentFrame() == 15)) return false;
		else return true;
		break;
	case CLIMBING_RIGHT:
		if (sprite->currentFrame() == 4) return false;
		else return true;
		break;
	case CLIMBING_LEFT:
		if (sprite->currentFrame() == 4) return false;
		else return true;
		break;
	case BUILDING_RIGHT:
		if (sprite->currentFrame() == 15) return false;
		else return true;
		break;
	case BUILDING_LEFT:
		if (sprite->currentFrame() == 15) return false;
		else return true;
		break;
	}
	return false;
}

bool Lemming::iDoRender() {
	switch (state) {
	case DEAD:
		if (sprite->currentFrame() == 15) return false;
		else return true;
		break;
	case VICTORIOUS:
		if (sprite->currentFrame() == 7) return false;
		else return true;
		break;
	}
	return true;
}

bool Lemming::iDoNotUpdate() {
	switch (state) {
	case DEAD:
		if (sprite->currentFrame() == 15) return true;
		else return false;
		break;
	case VICTORIOUS:
		if (sprite->currentFrame() == 7) return true;
		else return false;
		break;
	}
	return false;
}


void Lemming::setAnimations(ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/powers_quad.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.03125, 0.03125), &spritesheet, &shaderProgram);//  1/16 numero de frames de l'animació, 1/32 numero d'animacions
	sprite->setNumberAnimations(20);

	sprite->setAnimationSpeed(WALKING_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 32.0, 0.0));

	sprite->setAnimationSpeed(WALKING_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 32.0, 1.0/32.0));

	sprite->setAnimationSpeed(FALLING_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 32.0, 2.0/32.0));

	sprite->setAnimationSpeed(FALLING_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 32.0, 3.0/32.0));

	sprite->setAnimationSpeed(UMBRELLA_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(UMBRELLA_RIGHT, glm::vec2(float(i) / 32.0, 4.0/32.0));

	sprite->setAnimationSpeed(UMBRELLA_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(UMBRELLA_LEFT, glm::vec2(float(i) / 32.0, 5.0/32.0));

	sprite->setAnimationSpeed(PARACHUTE_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(PARACHUTE_RIGHT, glm::vec2(float(i) / 32.0, 6.0/32.0));

	sprite->setAnimationSpeed(PARACHUTE_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(PARACHUTE_LEFT, glm::vec2(float(i) / 32.0, 7.0/32.0));

	sprite->setAnimationSpeed(ENDWALK, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(ENDWALK, glm::vec2(float(i) / 32.0, 8.0/32.0));

	sprite->setAnimationSpeed(DIG, 8);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(DIG, glm::vec2(float(i) / 32.0, 9.0/32.0));

	sprite->setAnimationSpeed(STOPPER, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(STOPPER, glm::vec2(float(i) / 32.0, 10.0/32.0));

	sprite->setAnimationSpeed(CLIMBING_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(CLIMBING_RIGHT, glm::vec2(float(i) / 32.0, 11.0/32.0));

	sprite->setAnimationSpeed(CLIMBING_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 32.0, 13.0/32.0));

	sprite->setAnimationSpeed(BUILDING_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BUILDING_RIGHT, glm::vec2(float(i) / 32.0, 15.0/32.0));

	sprite->setAnimationSpeed(BUILDING_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BUILDING_LEFT, glm::vec2(float(i) / 32.0, 16.0/32.0));

	sprite->setAnimationSpeed(BASHING1_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(ENDWALK, glm::vec2(float(i) / 32.0, 17.0/32.0));

	sprite->setAnimationSpeed(BASHING2_RIGHT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BASHING2_RIGHT, glm::vec2(float(i) / 32.0, 16.0/32.0));

	sprite->setAnimationSpeed(BASHING1_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BASHING1_LEFT, glm::vec2(float(i) / 32.0, 17.0/32.0));

	sprite->setAnimationSpeed(BASHING2_LEFT, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(BASHING2_LEFT, glm::vec2(float(i) / 32.0, 18.0/32.0));

	sprite->setAnimationSpeed(DEATH, 12);
	for (int i = 0; i<16; i++)
		sprite->addKeyframe(DEATH, glm::vec2(float(i) / 32.0, 19.0/32.0));

	





}

void Lemming::give(int alt) {
	if (alt == 5) {
		engine->play2D("block.wav", false);
	}
	given = (AltState)alt;
}

bool Lemming::inTheBox(int x, int y)
{
	glm::ivec2 actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
	actualPos += glm::ivec2(7, 15);//sprite displacement
	if (x > actualPos.x + 4 || x < actualPos.x - 3) return false;
	if (y > actualPos.y  || y < actualPos.y - 11) return false;
	return true;
}

glm::ivec2 Lemming::position() {
	glm::ivec2 actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
	return actualPos += glm::ivec2(7, 15);//sprite displacement
}

int Lemming::report() {
	if (state == DEAD) return -1;
	else if (state == VICTORIOUS) return 1;
	else return 0;
}

bool Lemming::buildFinished() {
	++build;
	if (build >= 20) {
		build = 0;
		return true;
	}
	else return false;
}

void Lemming::displacement(int displ2) {
	sprite->position() -= glm::vec2(displ2, 0);
}




