#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "RedDoor.h"
#include "Game.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

enum RedDoorAnims
{
	OPEN_DOOR
};

void RedDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/red_door.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.10, 1), &spritesheet, &shaderProgram);//  1/10, 1/1
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(OPEN_DOOR, 12);
	for (int i = 0; i<10; i++)
		sprite->addKeyframe(OPEN_DOOR, glm::vec2(float(i) / 10, 0.0f));

	opener = 0;
	engine = irrklang::createIrrKlangDevice();
	engine->play2D("door.wav", false);
	sprite->changeAnimation(OPEN_DOOR);
	sprite->setPosition(initialPosition);
	
}

void RedDoor::update(int deltaTime)
{	
	if (sprite->currentFrame() < 9 && sprite->update(deltaTime) == 0)
		return;
}

void RedDoor::render()
{
	sprite->render();
}

void RedDoor::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

bool RedDoor::opened() {
	if (opener < 25) ++opener;
	return (opener < 25);
}


