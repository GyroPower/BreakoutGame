#include"resourceManager.h"


std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile,
	std::string name, const char* gShaderFile)
{
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];

}

Shader& ResourceManager::getShader(std::string name) {
	return shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile,
	const char* gShaderFile) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {
		std::ifstream vShaderFileSource(vShaderFile);
		std::ifstream fShaderFileSource(fShaderFile);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFileSource.rdbuf();
		fShaderStream << fShaderFileSource.rdbuf();

		vShaderFileSource.close();
		fShaderFileSource.close();
		
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr) {
			std::ifstream gShaderFileSource(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFileSource.rdbuf();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	
	return shader;
}

Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name) {
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
	Texture2D texture;

	if (alpha) {
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
	}

	int width, height, nrComponents;
	unsigned char* data = stbi_load(file, &width, &height, &nrComponents, 0);

	texture.Generate(width, height, data);

	return texture;
}

Texture2D& ResourceManager::getTexture(std::string name) {
	return textures[name];
}

void ResourceManager::clear() {
	//clearing all the data for rendering

	// deleting all the loaded textures
	for (auto& iter : textures)
		glDeleteTextures(1, &iter.second.id);
	// deleting all the shaders program for rendering
	for (auto& iter : shaders)
		glDeleteProgram(iter.second.id);
}

void ResourceManager::clearShader(std::string name) {
	glDeleteProgram(shaders[name].id);
}

void ResourceManager::clearTexture(std::string name) {
	glDeleteTextures(1, &textures[name].id);
}