#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	current_scene = MENU;
	engine = irrklang::createIrrKlangDevice();
	engine->play2D("sounds/highway.wav", true);
	sceneMenu.init();
}

bool Game::update(int deltaTime)
{
	int i;
	switch (current_scene) {
	case MENU:
		sceneMenu.update(deltaTime);
		break;

	case LEVEL1:
		sceneTuto.update(deltaTime);
		i = sceneTuto.report();
		if (i == 1) {
			sceneWin.init();
		}
		else if (i == -1) {
			sceneFail.init();
		}
		break;

	case LEVEL2:
		level2.update(deltaTime);
		i = level2.report();
		if (i == 1) {
			sceneWin.init();
		}
		else if (i == -1) {
			sceneFail.init();
		}
		break;

	case LEVEL3:
		level3.update(deltaTime);
		i = level3.report();
		if (i == 1) {
			sceneWin.init();
		}
		else if (i == -1) {
			sceneFail.init();
		}
		break;

	case CONTROLS:
		sceneControls.update(deltaTime);
		break;

	case WIN_LEVEL:
		sceneWin.update(deltaTime);
		break;

	case FAIL_LEVEL:
		sceneFail.update(deltaTime);
		break;

	case CREDITS:
		credits.update(deltaTime);
		break;

	default:
		break;
	}
		
		
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	switch (current_scene) {
	case MENU:
		sceneMenu.render();
		break;

	case LEVEL1:
		sceneTuto.render();
		break;

	case LEVEL2:
		level2.render();
		break;

	case LEVEL3:
		level3.render();
		break;

	case CONTROLS:
		sceneControls.render();
		break;

	case WIN_LEVEL:
		sceneWin.render();
		break;

	case FAIL_LEVEL:
		sceneFail.render();
		break;

	case CREDITS:
		credits.render();
		break;
	default:
		break;
	}
}

void Game::keyPressed(int key)
{
	switch (current_scene)
	{
	case MENU:
		if (key == 27) // Escape code
			bPlay = false;
		if (key == 48 || key == 49) { //0 and 1 code
			current_level = 1;
			current_scene = LEVEL1;
			engine->drop();
			sceneTuto.init();
		}
		if (key == 50) {			//2 code
			current_level = 2;
			current_scene = LEVEL2;
			engine->drop();
			level2.init();
		}
		if (key == 51) {			//3 code
			current_level = 3;
			current_scene = LEVEL3;
			engine->drop();
			level3.init();
		}
		if (key == 99) {			//c code
			current_scene = CONTROLS;
			sceneControls.init();
		}
		keys[key] = true;
		break;
	case CONTROLS:
		if (key == 27) {// Escape code
			current_scene = MENU;
			sceneMenu.init();
		}
	case WIN_LEVEL:
		if (key == 27) {// Escape code
			current_scene = MENU;
			sceneMenu.init();
		}
		if (key == 78) { //N code
			current_level = current_level + 1;
			if (current_level == 2) {
				current_scene = LEVEL2;
				engine->drop();
				level2.init();
			}
			else if (current_level == 3) {
				current_scene = LEVEL3;
				engine->drop();
				level3.init();
			}
		}

		break;
	case FAIL_LEVEL:
		if (key == 27) {// Escape code
			current_scene = MENU;
			sceneMenu.init();
		}
		if (key == 82) { //R code
			if (current_level == 1) {
				current_scene = LEVEL1;
				engine->drop();
				sceneTuto.init();
			}
			else if (current_level == 2) {
				current_scene = LEVEL2;
				engine->drop();
				level2.init();
			}
			else if (current_level == 3) {
				current_scene = LEVEL3;
				engine->drop();
				level3.init();
			}
		}
		break;
	case CREDITS:
		if (key == 27) {// Escape code
			current_scene = MENU;
			sceneMenu.init();
		}
		break;
	case LEVEL1:
		if (key == 99) sceneTuto.setPower(2);
		else if (key == 117) sceneTuto.setPower(4);
		else if (key == 101) sceneTuto.setPower(8);
		else if (key == 115) sceneTuto.setPower(5);
		else if (key == 107) sceneTuto.setPower(6);
		else if (key == 98) sceneTuto.setPower(3);
		else if (key == 100) sceneTuto.setPower(1);
		else if (key == 102) sceneTuto.setPower(9);
		else if (key == 112) sceneTuto.setPower(10);
		else if (key == 114) sceneTuto.setPower(11);
		break;
	case LEVEL2:
		if (key == 99) level2.setPower(2);
		else if (key == 117) level2.setPower(4);
		else if (key == 101) level2.setPower(8);
		else if (key == 115) level2.setPower(5);
		else if (key == 107) level2.setPower(6);
		else if (key == 98) level2.setPower(3);
		else if (key == 100) level2.setPower(1);
		else if (key == 102) level2.setPower(9);
		else if (key == 112) level2.setPower(10);
		else if (key == 114) level2.setPower(11);
		break;
	case LEVEL3:
		if (key == 99) level3.setPower(2);
		else if (key == 117) level3.setPower(4);
		else if (key == 101) level3.setPower(8);
		else if (key == 115) level3.setPower(5);
		else if (key == 107) level3.setPower(6);
		else if (key == 98) level3.setPower(3);
		else if (key == 100) level3.setPower(1);
		else if (key == 102) level3.setPower(9);
		else if (key == 112) level3.setPower(10);
		else if (key == 114) level3.setPower(11);
		break;
	default:
		break;
	}
	

}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	
	switch (current_scene) {
	case MENU:
		sceneMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case LEVEL1:
		sceneTuto.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case LEVEL2:
		level2.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case LEVEL3:
		level3.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case CONTROLS:
		sceneControls.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case WIN_LEVEL:
		sceneWin.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case FAIL_LEVEL:
		sceneFail.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;

	case CREDITS:
		credits.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;
	default:
		break;
	}
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		sceneMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		sceneMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





