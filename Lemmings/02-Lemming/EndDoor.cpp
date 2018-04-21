#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "EndDoor.h"
#include "Game.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

enum EndDoorAnims
{
	DOOR
};

void EndDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int mapDispl)
{
	spritesheet.loadFromFile("images/door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(36, 26), glm::vec2(0.1667, 1), &spritesheet, &shaderProgram);//  1/6, 1/1
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DOOR, 12);
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(DOOR, glm::vec2(float(i) / 6, 0.0f));

	engine = irrklang::createIrrKlangDevice();
	engine->play2D("sounds/fire.wav", true);
	sprite->changeAnimation(DOOR);
	sprite->setPosition(initialPosition);
	displ = mapDispl;
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

glm::ivec2 EndDoor::position()
{
	glm::ivec2 actualPos = sprite->position() + glm::vec2(displ, 0);//position + map displacement
	return actualPos += glm::ivec2(17, 25);//sprite displacement

}

void EndDoor::displacement(int displ2) {
	sprite->position() -= glm::vec2(displ2, 0);
	displ += displ2;
}