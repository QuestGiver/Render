#include "context.h"
#include "render.h"

#include "GLM/ext.hpp"

int main()
{
	context game;
	game.init(800, 600, "Bananna");

	vertex triVerts[] =
	{
		{{ -.5f, -.5f, 0, 1 }},
		{{   .5f,-.5f, 0, 1 }},
		{{    0,  .5f, 0, 1 }},

	};

	unsigned int triIndices[] = { 2,1,0 };

	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);

	const char * basicVert =
		"#version 410\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() { gl_Position = position; }";

	const char * mvpVert =
		"#version 430\n"
		"layout (location = 0) in vec4 position;\n"
		"layout (location = 0) uniform mat4 proj;\n"
		"layout (location = 1) uniform mat4 view;\n"
		"layout (location = 2) uniform mat4 model;\n"
		"void main() {gl_Position = proj * view * model * position;}";

	const char * basicFrag =
		"#version 330\n"
		"out vec4 vertColor;\n"
		"void main() {vertColor = vec4(1.0,0.0,0.0,1.0);}";



	shader basicShad = makeShader(basicVert, basicFrag);
	shader mvpShad = makeShader(mvpVert, basicFrag);

	glm::mat4 cam_proj = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 0, -1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	glm::mat4 triangle_model2 = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5,0.5,0.5));

	glm::mat4 * triArray = new glm::mat4[1000];

	for (int i = 0; i < triArray->length; i++)
	{
		triArray[i] = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5, 0.5, 0.5));
	}


	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		

		for (int i = 0; i < triArray->length; i++)
		{
			triArray[i] = glm::rotate(triArray[i], glm::radians((float)i), glm::vec3(0, 1, 0));
			setUniform(mvpShad, 0, cam_proj);
			setUniform(mvpShad, 1, cam_view);
			setUniform(mvpShad, 2, triArray[i]);
			draw(mvpShad, triArray[i]);
		}








		triangle_model = glm::rotate(triangle_model, glm::radians(FLT_MAX), glm::vec3(0, 1, 0));
		setUniform(mvpShad, 0, cam_proj);
		setUniform(mvpShad, 1, cam_view);
		setUniform(mvpShad, 2, triangle_model);

		draw(mvpShad, triangle);

		setUniform(mvpShad, 2, triangle_model2);
		draw(mvpShad, triangle);

	}

	game.term();

	delete[] triArray;

	return 0;
}