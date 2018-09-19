#include "render.h"
#include "GLM/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <cstdio>
#include <cassert>

geometry makeGeometry(vertex * verts, size_t vertCount, unsigned int * indices, size_t indxCount)
{
	// create instance of geometry
	geometry newGeo ={};
	newGeo.size = indxCount;
	//gen buffers
	
	glGenVertexArrays(1, &newGeo.vao);
	glGenBuffers(1, &newGeo.vbo);
	glGenBuffers(1, &newGeo.ibo);
	// bind buffs
	glBindVertexArray(newGeo.vao);
	glBindBuffer(GL_ARRAY_BUFFER, newGeo.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGeo.ibo);
	// populate buffers
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//describe vertex data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)16);

	// unbind buffers

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// return geometry
	return newGeo;
}

void freeGeometry(geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vbo);

	geo = {};
}

texture loadTexture(const char * imagePath)
{
	int imageWidth, imageHeight, imageFormat;
	imageWidth = imageHeight = imageFormat = 1;
	unsigned char *rawPixelData = nullptr;

	//load image
	stbi_set_flip_vertically_on_load(true);
	rawPixelData = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	//get format/spec/whatever
	texture newTex = makeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);
	//pass it opengl
	stbi_image_free(rawPixelData);
	//destroy any other data
	

	return newTex;
}

shader makeShader(const char * vertSource, const char * fragSource)
{
	//make shader obj
	shader newShad = {};
	newShad.program = glCreateProgram();

	//create shader
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	//compile shader
	glShaderSource(vert, 1, &vertSource, 0);
	glShaderSource(frag, 1, &fragSource, 0);
	glCompileShader(vert);
	glCompileShader(frag);

	//attach shader
	glAttachShader(newShad.program, vert);
	glAttachShader(newShad.program, frag);

	//link shader
	glLinkProgram(newShad.program);

	//delete shader
	glDeleteShader(vert);
	glDeleteShader(frag);

	//return shadr obj
	return newShad;
	//
	//
}

void freeShader(shader &shad)
{
	glDeleteProgram(shad.program);
	shad = {};
}

texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char * pixels)
{
	GLenum oglFormat = 0;
	switch (channels)
	{
	case 1:
		oglFormat = GL_RED;
		break;
	case 2:
		oglFormat = GL_RG;
		break;
	case 3:
		oglFormat = GL_RGB;
		break;
	default:
	case 4:
		oglFormat = GL_RGBA;
		break;
	}

	texture newTex = { 0, width, height, channels };

	glGenTextures(1, &newTex.handle);

	glBindTexture(GL_TEXTURE_2D, newTex.handle);
	
	glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return newTex;
}

texture freeTexture(texture & tex)
{

	glDeleteTextures(1, &tex.handle);
	tex = {};

	return tex;
}

void draw(const shader &shad, const geometry &geo)
{
	glUseProgram(shad.program);
	glBindVertexArray(geo.vao);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void setUniform(const shader &shad, GLuint location, const glm::mat4 &val)
{
	glProgramUniformMatrix4fv(shad.program, location, 1, GL_FALSE, glm::value_ptr( val));
}

void setUniform(const shader & shad, GLuint location, const texture & value, GLuint textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, value.handle);
	glProgramUniform1i(shad.program, location, textureSlot);
}
