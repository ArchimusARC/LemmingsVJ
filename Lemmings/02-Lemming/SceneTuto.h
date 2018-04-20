#ifndef _SCENETUTO_INCLUDE
#define _SCENETUTO_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "RedDoor.h"
#include "EndDoor.h"
#include "Barra.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneTuto
{

public:
	SceneTuto();
	~SceneTuto();

	void init();
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void initiateNextLemming();

private:
	Texture colorTexture, colorTexturePintar;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	int lemmingsInitiated;
	bool accel;
	bool pause;
	glm::mat4 projection;
	Lemming lemmings[10];
	int livingLemmings;
	int victoriousLemmings;
	int displ;
	RedDoor red_door;
	EndDoor goal;
	Barra toolbar;

};


#endif // _SCENETUTO_INCLUDE

