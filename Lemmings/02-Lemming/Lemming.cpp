#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_RIGHT, FALLING_LEFT, FLOATING_LEFT, FLOATING_RIGHT,
	CLIMBING_RIGHT, CLIMBING_LEFT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	given = DIGGER;
	setAnimations(shaderProgram);
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;
	if (given == BLOCKER && (state != FALLING_LEFT_STATE || state != FALLING_RIGHT_STATE)) state = BLOCKING;
	else if (given == DIGGER && (state == WALKING_LEFT_STATE  || state == WALKING_RIGHT_STATE)) state = DIGGING;
	bool col;
	glm::ivec2 pollasentuboca;
	switch (state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if (fall > 0){
			sprite->position() += glm::vec2(0, fall);
			fallDistance += fall;
			if (given == PARACHUTE) sprite->changeAnimation(FLOATING_LEFT);
			pollasentuboca = sprite->position();
			if (pollasentuboca.y + fall >= 170) {
				state = DEAD;
			}
		}
		else{
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			if (fallDistance + fall > 50 && given != PARACHUTE) {
				state = DEAD;
			}
			else given = NONE;
		}
		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if (fall > 0){
			sprite->position() += glm::vec2(0, fall);
			fallDistance += fall;
			if (given == PARACHUTE) sprite->changeAnimation(FLOATING_RIGHT);
			pollasentuboca = sprite->position();
			if (pollasentuboca.y + fall >= 170) {
				state = DEAD;
			}
		}
		else{
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			if (fallDistance + fall > 50 && given != PARACHUTE) {
				state = DEAD;
			}
			else given = NONE;
		}
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		col = collision();
		if(col && given == CLIMBER)
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
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2){
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
			if(fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else{
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
				fallDistance = fall;
			}
		}
		break;
	case DEAD:
		sprite->setPosition(glm::vec2(32,170));
		break;
	case DIGGING:
		pollasentuboca = sprite->position() + glm::vec2(360, 0);//position + map displacement
		pollasentuboca += glm::ivec2(7, 15);//sprite displacement
		for (int i = 0; i < 5; ++i) {
			mask->setPixel(pollasentuboca.x + i - 2, pollasentuboca.y + 1, 0);
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
		sprite->position() += glm::vec2(0, -1);

		break;
	case CLIMBING_RIGHT:

		break;
	case BASHING_LEFT:
		if (continue_bashing(0)) {
			pollasentuboca = sprite->position() + glm::vec2(360, 0);//position + map displacement
			pollasentuboca += glm::ivec2(7, 15);//sprite displacement
			for (int i = 0; i < 10; ++i) {
				mask->setPixel(pollasentuboca.x - 1, pollasentuboca.y - i, 0);
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
		if (continue_bashing(1)) {
			pollasentuboca = sprite->position() + glm::vec2(360, 0);//position + map displacement
			pollasentuboca += glm::ivec2(7, 15);//sprite displacement
			for (int i = 0; i < 10; ++i) {
				mask->setPixel(pollasentuboca.x + 2, pollasentuboca.y - i, 0);
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

		break;
	case BUILDING_LEFT:

		break;
	case BUILDING_RIGHT:

		break;
	}
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(360, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(360, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::continue_bashing(int right) {
	glm::ivec2 posBase = sprite->position() + glm::vec2(360, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15);
	for (int i = 0; i < 10; ++i) {
		if (mask->pixel(posBase.x -1 + 4 * right, posBase.y + i) == 0)
			return true;
	}
	return false;
}



void Lemming::setAnimations(ShaderProgram &shaderProgram) {
	spritesheet.loadFromFile("images/lemming_Idle.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);//  1/8 numero de frames de l'animació, 1/4 numero d'animacions
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(WALKING_RIGHT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.0/4.0f));

	sprite->setAnimationSpeed(WALKING_LEFT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.0 /4.0f));

	sprite->setAnimationSpeed(FALLING_RIGHT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 2.0 /4.0f));

	sprite->setAnimationSpeed(FALLING_LEFT, 12);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 8, 3.0/4.0f));
}



