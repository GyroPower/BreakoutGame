#pragma once
#include<vector>
#include"../gameObject/gameObject.h"
#include"../resourceManager/resourceManager.h"


class GameLevel {
public:
	
	std::vector<GameObject> bricks;

	GameLevel();


	void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void draw(spriteRenderer& renderer);

	bool isCompleted();

private:

	void init(std::vector<std::vector<unsigned int>> tileDelta,
		unsigned int lvlWidth, unsigned int lvlHeight);
};