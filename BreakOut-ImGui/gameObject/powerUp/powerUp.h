#pragma once
#include"../gameObject.h"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

enum PowerUpType {
	CHAOS,
	CONFUSE,
	INCREASE,
	PASSTHROUGH,
	SPEED,
	STICKY,
};

class PowerUp : public GameObject {
public:
	PowerUpType type;
	float duration;
	bool activated;

	PowerUp(PowerUpType type, glm::vec3 color, float duration,
		glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY),
		type(type), duration(duration), activated(false)
	{ }
};

