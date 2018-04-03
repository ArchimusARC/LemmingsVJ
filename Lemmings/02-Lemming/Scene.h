#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "RedDoor.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define TIME_PER_FRAME 1000.f / 30.f // Approx. 30 fps



class Scene
{

public:
	Scene();
	~Scene();

	void init(int firstFrame);
	void update(int deltaTime, int frameCount);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);

private:
	Texture colorTexture, colorTexturePintar;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	Lemming lemmings[4];
	RedDoor red_door;
	int actualFrame;

};


#endif // _SCENE_INCLUDE

