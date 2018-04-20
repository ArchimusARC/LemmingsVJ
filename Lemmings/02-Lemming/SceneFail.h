#ifndef _SCENEFAIL_INCLUDE
#define _SCENEFAIL_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneFail
{

public:
	SceneFail();
	~SceneFail();

	void init();
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENEFAIL_INCLUDE
