#pragma once
#include"../gameObject.h"

class ballObject : public GameObject {
public:
	float radius;
	bool stuck;
	bool sticky;
	bool passThrought;

	ballObject();
	ballObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(float dt, unsigned int windowWidth);
	void reset(glm::vec2 position, glm::vec2 velocity);
};
