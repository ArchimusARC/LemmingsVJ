#ifndef _SCENECREDITS_INCLUDE
#define _SCENECREDITS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "../irrKlang-1.6.0/include/irrKlang.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneCredits
{

public:
	SceneCredits();
	~SceneCredits();

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
	irrklang::ISoundEngine* engine;

};


#endif // _SCENECREDITS_INCLUDE