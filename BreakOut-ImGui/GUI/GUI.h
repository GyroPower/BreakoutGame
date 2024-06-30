#pragma once
#include<imgui.h>
#include<imgui_internal.h>
#include"../game/game.h"

class GUI {
	
private:
	unsigned int w_width;
	unsigned int w_height;

	

public:

	bool startGame;

	GUI(unsigned int w_width, unsigned int w_height);
	~GUI();

	void mainMenuWindow(GameState& state, unsigned int& level);
	void showKeysPressed(bool& show_keys_pressed);





};