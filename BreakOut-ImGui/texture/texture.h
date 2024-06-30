#pragma once
#include<glad/gl.h>


class Texture2D {
public:
	unsigned int id;
	int width, height, nrComponents;

	unsigned int internalFormat;
	unsigned int imageFormat;
	unsigned int wrapS;
	unsigned int wrapT;
	unsigned int filterMin;
	unsigned int filterMax;

	Texture2D();

	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	void Bind();

	unsigned int getId() const;

	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getNrComponents() const;
};
