#include "render.h"
#include "GLM/gtc/type_ptr.hpp"

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