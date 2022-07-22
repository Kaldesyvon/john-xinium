#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <stb/stb_image.h>
#include <glad/glad.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint uint);
	void Bind();
	void Unbind();
	void Delete();
};


#endif // !TEXTURE_CLASS_H


