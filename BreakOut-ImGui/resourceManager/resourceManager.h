#pragma once
#include<map>
#include<string>
#include<glad/include/glad/gl.h>

#include<stb_image/stb_image.h> 

#include"../shaderProgram/shaderProgram.h"
#include"../texture/texture.h"

class ResourceManager {

public:

	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;
	
	static Shader loadShader(const char* vShaderFile, const char* fShaderFile,
		std::string name, const char* gShaderFile = nullptr);
	static Shader& getShader(std::string name);

	static Texture2D loadTexture(const char* file, bool alpha, std::string name);
	static Texture2D& getTexture(std::string name);

	static void clear();

	static void clearTexture(std::string name);
	static void clearShader(std::string name);

private:

	ResourceManager() {};

	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile,
		const char* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);
};