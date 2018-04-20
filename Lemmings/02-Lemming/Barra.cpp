#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Barra.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


void Barra::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int displacement)
{
	state = NONE;
	spritesheet.loadFromFile("images/toolbar/toolbarfull.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(144.0, 18.0), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NONE, 12);
	for (int i = 0; i<1; i++)
		sprite->addKeyframe(NONE, glm::vec2(0.0f, 0.0f));
	displ = displacement;
	sprite->changeAnimation(NONE);
	sprite->setPosition(initialPosition);
	position = initialPosition;
}


void Barra::render()
{
	sprite->render();
}

int Barra::checkState(int x, int y) {
	int altx = x / 3 + displ;
	if (position.y > y && y < position.y + 18) {
		if (altx < position.x + 12 && altx > 0) {
			state = NONE;
			return 11;
		}
		else if (altx < position.x + 24 && altx > position.x + 12) {
			state = NONE;
			return 12;
		}
		else if (altx < position.x + 36 && altx > position.x + 24) {
			if (state != CLIMBER) state = CLIMBER;
			else state = NONE;
		}
		else if (altx < position.x + 48 && altx > position.x + 36) {
			if (state != PARACHUTE) state = PARACHUTE;
			else state = NONE;
		}
		else if (altx < position.x + 60 && altx > position.x + 48) {
			if (state != BLOCKER) state = BLOCKER;
			else state = NONE;
		}
		else if (altx < position.x + 72 && altx > position.x + 60) {
			if (state != BUILDER) state = BUILDER;
			else state = NONE;
		}
		else if (altx < position.x + 84 && altx > position.x + 72) {
			if (state != BASHER) state = BASHER;
			else state = NONE;
		}
		else if (altx < position.x + 96 && altx > position.x + 84) {
			if (state != DIGGER) state = DIGGER;
			else state = NONE;
		}
		else if (altx < position.x + 108 && altx > position.x + 96) {
			if (state != EXPLODER) state = EXPLODER;
			else state = NONE;
		}
		else if (altx < position.x + 120 && altx > position.x + 108) {
			if (state != ACCELERATE) state = ACCELERATE;
			else state = NONE;
		}
		else if (altx < position.x + 132 && altx > position.x + 120) {
			if (state != PAUSE) state = PAUSE;
			else state = NONE;
		}
		else if (altx < position.x + 144 && altx > position.x + 132) state = EXPLOSION;
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
