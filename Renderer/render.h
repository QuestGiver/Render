#include "glew\glew.h"
#include "GLM/glm.hpp"


struct vertex
{
	glm::vec4 pos;
	glm::vec2 uv;
};


struct geometry
{
	GLuint vao, vbo, ibo; // buffers

	GLuint size; // index count
};

struct shader
{
	GLuint program;





};

struct texture
{
	GLuint handle;
	unsigned int width, height, channels;
};

geometry makeGeometry(vertex * verts, size_t vertcount, unsigned int * indices, size_t indxCount);

void freeGeometry(geometry & geo);

texture loadTexture(const char * imagePath);

shader makeShader(const char * vertSource, const char * fragSource);

void freeShader(shader &shad);

texture makeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char * pixels);

texture freeTexture(texture &tex);

void draw(const shader &shad, const geometry &geo);

void setUniform(const shader &shad, GLuint location, const glm::mat4 &val);

void setUniform(const shader &shad, GLuint location,const texture &value, GLuint textureSlot);