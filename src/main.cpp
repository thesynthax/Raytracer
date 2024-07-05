#include <iostream>
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "scene.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const std::string VERT_PATH = "/home/thesynthax/projects/cpp/raytracer/src/vertex.glsl";
const std::string FRAG_PATH = "/home/thesynthax/projects/cpp/raytracer/src/fragment.glsl";

int main() {

    //Window Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GraphicsEngine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
    double mouseX, mouseY;
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //Vertex Data and Vertex buffer Initialization
    GLfloat vertices[] =
	{
        -1.0f, -1.0f, 0.0f, 
        1.0f, -1.0f, 0.0f, 
        1.0f, 1.0f, 0.0f, 
        -1.0f, -1.0f, 0.0f, 
        1.0f, 1.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f, 
	};
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    //Shader Initialization and main code loop
    Shader shader(VERT_PATH, FRAG_PATH);
    shader.use();
    initializeUniforms(shader);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        updateUniforms(shader);

        shader.setVec2("u_screenPixels", SCR_WIDTH, SCR_HEIGHT);
        shader.setFloat("u_aspectRatio", (float)SCR_WIDTH/(float)SCR_HEIGHT);
        
        float timeValue = glfwGetTime();
        shader.setFloat("u_time", timeValue);
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        shader.setVec2("u_mousePos", (float)(mouseX/SCR_WIDTH), 1.0f - (float)(mouseY/SCR_HEIGHT));

		glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    //Deletion of vertex data and deactivation of shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    shader.deactivate();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
