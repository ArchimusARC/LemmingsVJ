#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "SceneTuto.h"
#include "SceneMenu.h"
#include "SceneControls.h"
#include "SceneLevel2.h"
#include "SceneLevel3.h"
#include "SceneWin.h"
#include "SceneFail.h"
#include "SceneCredits.h"
#include "../irrKlang-1.6.0/include/irrKlang.h"



// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	enum Scenes {
		MENU, LEVEL1, LEVEL2, LEVEL3, CREDITS, CONTROLS, WIN_LEVEL, FAIL_LEVEL
	};

	bool bPlay;   // Continue to play game?
	Scene scene;
	SceneTuto sceneTuto;
	SceneMenu sceneMenu;
	SceneControls sceneControls;
	SceneLevel2 level2;
	SceneLevel3 level3;
	SceneWin sceneWin;
	SceneFail sceneFail;
	SceneCredits credits;
	Scenes current_scene;
	int current_level;
	irrklang::ISoundEngine* engine;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states

	
};


#endif // _GAME_INCLUDE


