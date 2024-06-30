#pragma once
//#include<glm/gtc/matrix_transform.hpp>

#include"../shaderProgram/shaderProgram.h"
#include"../texture/texture.h"

class spriteRenderer {

public:
	spriteRenderer(Shader& shader);
	~spriteRenderer();

	void drawSprite(Texture2D& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

private:
	Shader shader;
	unsigned int quadVAO;

	void initRenderData();

};