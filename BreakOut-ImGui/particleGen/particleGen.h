#pragma once
#include <glm/glm.hpp>
#include<vector>
#include<random>
#include"../shaderProgram/shaderProgram.h"
#include"../texture/texture.h"
#include"../gameObject/gameObject.h"

struct particle
{
	glm::vec2 position, velocity;
	glm::vec4 color;
	float life;

	particle()
		:position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}

};

class GenParticle {
public:
	std::vector<particle> particles;
	

	GenParticle(Shader& shader, Texture2D& texture, unsigned int nrParticles);
	
	void respawnParticle(particle& particle, GameObject& gameObject, glm::vec2 offset);
	void updateParticle(float dt,GameObject& gameObject, glm::vec2 offset, 
		unsigned int new_nrParticles = 1);
	void render();

	int firstUnusedParticle();

private:
	Shader shader;
	Texture2D texture;
	int nrParticles;
	int lastUnusedParticle;
	unsigned int VAO;
	void initRenderData();
	

};