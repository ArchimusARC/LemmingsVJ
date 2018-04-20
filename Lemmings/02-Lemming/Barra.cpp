#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Barra.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


void Barra::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = NONE;
	spritesheet.loadFromFile("images/toolbar/toolbarfull.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(144.0,18), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	
		sprite->setAnimationSpeed(NONE, 12);
		for(int i=0; i<1; i++)
			sprite->addKeyframe(NONE, glm::vec2( 0.0f, 0.0f));
		
	sprite->changeAnimation(NONE);
	sprite->setPosition(initialPosition);
	position = initialPosition;
}

void Barra::update(ShaderProgram &shaderProgram)
{
	if (lastState != state && state > (BarraState)0  && state < EXPLOSION) {
		spritesheetboto.loadFromFile("images/toolbar/toolbarfull.png", TEXTURE_PIXEL_FORMAT_RGBA);
		spritesheetboto.setMinFilter(GL_NEAREST);
		spritesheetboto.setMagFilter(GL_NEAREST);
		spriteboto = Sprite::createSprite(glm::ivec2(52, 74), glm::vec2(0.0, 0.0), &spritesheet, &shaderProgram);
		spriteboto->setNumberAnimations(1);

		spriteboto->setAnimationSpeed(state, 12);
		for (int i = 0; i<1; i++)
			spriteboto->addKeyframe(state, glm::vec2(0.0f, 0.0f));

		spriteboto->changeAnimation(state);
		glm::vec2 temp;
		switch (state) {
		case CLIMBER:
			temp = position + glm::vec2(104,0);
			spriteboto->setPosition(temp);
			break;
		case PARACHUTE:
			temp = position + glm::vec2(156, 0);
			spriteboto->setPosition(temp);
			break;
		case BLOCKER:
			temp = position + glm::vec2(208, 0);
			spriteboto->setPosition(temp);
			break;
		case BUILDER:
			temp = position + glm::vec2(260, 0);
			spriteboto->setPosition(temp);
			break;
		case BASHER:
			temp = position + glm::vec2(312, 0);
			spriteboto->setPosition(temp);
			break;
		case DIGGER:
			temp = position + glm::vec2(364, 0);
			spriteboto->setPosition(temp);
			break;
		case EXPLODER:
			temp = position + glm::vec2(416, 0);
			spriteboto->setPosition(temp);
			break;
		}
	}
}

void Barra::render()
{
	sprite->render();
	if (state != NONE && state != ACCELERATE && state != PAUSE && state != EXPLOSION) spriteboto->render();
}

int Barra::checkState(int x, int y) {
	int altx = x + displ;
	if (position.y > y && y < position.y + 74) {
		if (altx < position.x + 156 && altx > position.x + 104) {
			if (state != CLIMBER) state = CLIMBER;
			else state = NONE;
		}
		else if (altx < position.x + 208 && altx > position.x + 156) {
			if (state != PARACHUTE) state = PARACHUTE;
			else state = NONE;
		}
		else if (altx < position.x + 260 && altx > position.x + 208) {
			if (state != BLOCKER) state = BLOCKER;
			else state = NONE;
		}
		else if (altx < position.x + 312 && altx > position.x + 260) {
			if (state != BUILDER) state = BUILDER;
			else state = NONE;
		}
		else if (altx < position.x + 364 && altx > position.x + 312) {
			if (state != BASHER) state = BASHER;
			else state = NONE;
		}
		else if (altx < position.x + 416 && altx > position.x + 364) {
			if (state != DIGGER) state = DIGGER;
			else state = NONE;
		}
		else if (altx < position.x + 468 && altx > position.x + 416) {
			if (state != EXPLODER) state = EXPLODER;
			else state = NONE;
		}
		else if (altx < position.x + 520 && altx > position.x + 468) {
			if (state != ACCELERATE) state = ACCELERATE;
			else state = NONE;
		}
		else if (altx < position.x + 572 && altx > position.x + 520) {
			if (state != PAUSE) state = PAUSE;
			else state = NONE;
		}
		else if (altx < position.x + 634 && altx > position.x + 572) state = EXPLOSION;
	}
	return state;
}

void Barra::setDispl(int mapDispl) {
	displ = mapDispl;
}

void Barra::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Barra::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
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

bool Barra::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}







