#include"texture.h"


Texture2D::Texture2D()
	:width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), wrapS(GL_REPEAT),
	wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR), nrComponents(0)
{
	glGenTextures(1, &this->id);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data) {
	this->width = width;
	this->height = height;

	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height,
		0, this->imageFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMin);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() {
	glBindTexture(GL_TEXTURE_2D, this->id);

}

unsigned int Texture2D::getId() const {
	return this->id;
}

unsigned int Texture2D::getWidth() const {
	return this->width;
}

unsigned int Texture2D::getHeight() const {
	return this->height;
}

unsigned int Texture2D::getNrComponents() const{
	return this->nrComponents;
}