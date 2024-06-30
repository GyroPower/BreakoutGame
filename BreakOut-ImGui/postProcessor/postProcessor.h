#pragma once
#include<glad/include/glad/gl.h>
#include"../shaderProgram/shaderProgram.h"
#include"../texture/texture.h"
#include<iostream>

class PostProcessor {

public:

	Shader postProcessingShader;
	Texture2D texture;

	unsigned int width, height;

	bool shake, confuse, chaos;

	PostProcessor(Shader& shader, unsigned int width, unsigned int height);

	void beginRender();
	void endRender();

	void render(float time);

private:

	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;

	void initRenderData();
};