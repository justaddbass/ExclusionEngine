#include "Texture.h"
#include <cassert>
#include <stdio.h>


Texture::Texture(const std::string& filename)
{
	int width, height, numComponents;
	unsigned char* imageData = LoadImage(filename.c_str(), &width, &height, &numComponents, 4);

	if(imageData == nullptr)
		printf("Texture loading failed: %s\n", filename.c_str());

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	//stbi_image_free(imageData);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
