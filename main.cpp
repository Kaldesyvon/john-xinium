#include <iostream>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Texture.h"
#include"VAO.h"
#include"EBO.h"
#include"Camera.h"

static const int _width = 800;
static const int _height = 800;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{ //   COORDINATES          /     COLORS          /    texcoords
		-0.5f,  0.0f,  0.5f,     1.0f, 1.0f,  1.0f,    0.0f,  0.0f,
		-0.5f,  0.0f, -0.5f,     1.0f, 0.0f,  0.0f,	   1.0f,  0.0f,
		 0.5f,  0.0f, -0.5f,     0.0f, 1.0f,  0.0f,    0.0f,  0.0f,
		 0.5f,  0.0f,  0.5f,     0.0f, 0.0f,  1.0f,    1.0f,  0.0f,
		 0.0f,  0.8f,  0.0f,     0.0f, 0.0f,  1.0f,    0.5f,  1.0f,
		 
	};

	GLuint indices[] =
	{
		0,1,2,
		0,2,3,
		0,1,4,
		1,2,4,
		2,3,4,
		3,0,4
	};

	GLfloat lightVertices[] =
	{
		-0.1f, -0.1f,   0.1f,
		-0.1f, -0.1f,  -0.1f,
		 0.1f, -0.1f,  -0.1f,
		 0.1f, -0.1f,   0.1f,
		-0.1f,  0.1f,   0.1f,
		-0.1f,  0.1f,  -0.1f,
		 0.1f,  0.1f,  -0.1f,
		 0.1f,  0.1f,   0.1f,
	};

	GLuint lightIndices[] =
	{
		0,1,2,
		0,2,3,
		0,4,7,
		0,7,3,
		3,7,6,
		3,6,2,
		2,6,5,
		2,5,1,
		1,5,4,
		1,4,0,
		4,5,6,
		4,6,7
	};

	GLFWwindow* window = glfwCreateWindow(_width, _height, "window", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, _width, _height);

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Shader lightShader("light.vert", "light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

	
	
	Texture brick("front.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brick.texUnit(shaderProgram, "tex0", 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	//float rotation = 0.0f;
	//double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	Camera camera(_width, _height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		/*double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1.0f / 60.0f)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}*/

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.f);
		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");
		
		brick.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();

	shaderProgram.Delete();
	brick.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}