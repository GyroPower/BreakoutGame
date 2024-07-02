#pragma once
#include<imgui.h>
#include<imgui_internal.h>
#include"../game/game.h"

class GUI {
	
private:
	unsigned int w_width;
	unsigned int w_height;
	int debug_vec_count = 0;

	

public:

	GUI(unsigned int w_width, unsigned int w_height);
	~GUI();

	void mainMenuWindow(GameState& state, unsigned int& level, bool& close);
	void showKeysPressed(bool& show_keys_pressed);
	void showVec2(std::string vec_name, glm::vec2& vec);
	void showOneVariable(std::string variableName, std::string var, bool move = true,
		int posX = 0, int posY = 0, float scale = 1.0f, float alpha = 1.0f);





};