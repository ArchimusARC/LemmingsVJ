#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init(int firstFrame)
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = { glm::vec2(360.f / 2048.0, 0.f), glm::vec2((360.f + 320.f) / 2048.0f, 160.f / 256.0f) };
	//tricky 6 1211x160
	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/tricky6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	colorTexturePintar.loadFromFile("images/texturadefons2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexturePintar.setMinFilter(GL_NEAREST);
	colorTexturePintar.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/tricky6_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	actualFrame = firstFrame;
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	for (int i = 0; i < 4; ++i){
		lemmings[i].init(glm::vec2(60 + i*15, 30), simpleTexProgram);
		lemmings[i].setMapMask(&maskTexture);
	}
	
	red_door.init(glm::vec2(65, 10), simpleTexProgram, actualFrame);
}

unsigned int x = 0;

void Scene::update(int deltaTime, int frameCount)
{
	currentTime += deltaTime;
	for (int i = 0; i < 4; ++i){
		lemmings[i].update(deltaTime);
	}
	if(red_door.opened(frameCount-actualFrame))red_door.update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	//map->render(maskedTexProgram, colorTexture, maskTexture);
	maskedTexProgram.setUniform1i("fons", 1);
	map->render(maskedTexProgram, colorTexturePintar, maskTexture);
	maskedTexProgram.setUniform1i("fons", 0);
	map->render(maskedTexProgram, colorTexture, maskTexture);


	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	for (int i = 0; i < 4; ++i){
		lemmings[i].render();
	}
	red_door.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if(bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 360;
	posY = mouseY/3;

	for (int y = max(0, posY - 4); y <= min(maskTexture.height() - 1, posY + 4); y++){
		for (int x = max(0, posX - 4); x <= min(maskTexture.width() - 1, posX + 4); x++){
			if (sqrt(pow(x - posX, 2) + pow(y - posY, 2)) < 4) maskTexture.setPixel(x, y, 0);
		}
	}
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 360;
	posY = mouseY/3;

	for (int y = max(0, posY - 4); y <= min(maskTexture.height() - 1, posY + 4); y++){
		for (int x = max(0, posX - 4); x <= min(maskTexture.width() - 1, posX + 4); x++){
			if (sqrt(pow(x - posX, 2) + pow(y - posY, 2)) < 4) maskTexture.setPixel(x, y, 255);
		}
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



