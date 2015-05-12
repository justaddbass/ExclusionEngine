#ifndef EE_TEXTURE_H
#define EE_TEXTURE_H

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
#endif

#include <string>

class Texture
{
public:
	Texture(const std::string& filename);

	void Bind(unsigned int unit);
	~Texture();
private:
	GLuint mTexture;

};
#endif
