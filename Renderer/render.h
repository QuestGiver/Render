#include "glew\glew.h"
#include "GLM/glm.hpp"

struct vertex
{
	glm::vec4 pos;
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

geometry makeGeometry(vertex * verts, size_t vertcount, unsigned int * indices, size_t indxCount);

void freeGeometry(geometry & geo);

shader makeShader(const char * vertSource, const char * fragSource);

void freeShader(shader &shad);

void draw(const shader &shad, const geometry &geo);

void setUniform(const shader &shad, GLuint location, const glm::mat4 &val);