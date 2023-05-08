#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include "stb_image.h"

// currently use GL_REPEAT+GL_LINEAR_MIPMAP_LINEAR+GL_LINEAR, generate mipmap by default
// only support 2d texture
class Texture {
private:
	unsigned int textureId;
	unsigned int textureUnit;
	int width, height, nrChannels;

public:
	enum TexType {TEX2D, TEX3D} texType;
	enum DataType {JPG, PNG} dataType;
	enum AttrType {DIFFUSE, SPECULAR} attrType;
	std::string dataPath;
	Texture(TexType texType, DataType dataType, std::string path, unsigned int targetUnit);
	void BindTexUnit();
	void BindTexUnit(unsigned int unit);
	unsigned int GetTexId() { return textureId; }
	unsigned int GetTexUnit() { return textureUnit; }
};

// construction does not activate texture unit
Texture::Texture(Texture::TexType texType, Texture::DataType dataType, std::string path, unsigned int targetUnit = 0) {
	textureUnit = targetUnit;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	// if use stb, flip by default
	stbi_set_flip_vertically_on_load(false);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			dataType == JPG ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}


void Texture::BindTexUnit() {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::BindTexUnit(unsigned int unit) {
	textureUnit = unit;
	BindTexUnit();
}

#endif
