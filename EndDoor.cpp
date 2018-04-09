#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "EndDoor.h"
#include "Game.h"

enum EndDoorAnims
{
	DOOR
};

void EndDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(36, 26), glm::vec2(0.1667, 1), &spritesheet, &shaderProgram);//  1/6, 1/1
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DOOR, 12);
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(DOOR, glm::vec2(float(i) / 6, 0.0f));


	sprite->changeAnimation(DOOR);
	sprite->setPosition(initialPosition);
}

void EndDoor::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void EndDoor::render()
{
	sprite->render();
}

void EndDoor::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}
