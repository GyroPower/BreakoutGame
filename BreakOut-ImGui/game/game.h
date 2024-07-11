#pragma once
#include<vector>
#include<glm/gtc/matrix_transform.hpp>
#include"../spriteRenderer/spriteRenderer.h"
#include"../shaderProgram/shaderProgram.h"

#include"../gameLevel/gameLevel.h"
#include"../resourceManager/resourceManager.h"
#include"../gameObject/powerUp/powerUp.h"
#include"../gameObject/ballObject/ballObject.h"
#include<GLFW/glfw3.h>


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_PAUSE_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};



class Game {
public:

	std::vector<GameLevel> levels;
	std::vector<PowerUp> powerUps;

	unsigned int level;
	GameState state;
	bool keys[1024];
	bool keysProcessed[1024];
	bool show_keys_pressed;
	bool initGame;
	bool close;
	GLFWwindow* currentWindow;

	unsigned int width, height;
	unsigned int lives;

	Game(unsigned int width, unsigned int height);

	~Game();

	void init();

	void processInput(float dt);
	void update(float dt, GLFWwindow* window);
	void render();
	void renderGUI();
	void resetLevel();
	void resetPlayer();
	void spawnPowerUps(GameObject& block);
	void updatePowerUps(float dt);

	void doCollisions();
};