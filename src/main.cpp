#include <iostream>
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const std::string VERT_PATH = "/home/thesynthax/projects/cpp/raytracer/src/vertex.glsl";
const std::string FRAG_PATH = "/home/thesynthax/projects/cpp/raytracer/src/fragment.glsl";

GLFWcursorposfun cursorPosCallback(GLFWwindow* window, double mouseX, double mouseY) {
    glfwGetCursorPos(window, &mouseX, &mouseY);

        /*
		if (glfwGetKey(window, GLFW_KEY_E)) {
			Scene::mousePlace(mouseX, mouseY, SCR_WIDTH, SCR_HEIGHT, Scene::cameraPosition, rotationMatrix);
		}
		else {
			Scene::selectHovered(mouseX, mouseY, SCR_WIDTH, SCR_HEIGHT, Scene::cameraPosition, rotationMatrix);
		}*/
    return 0;
}

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
    //glfwSetCursorPosCallback(window, cursorPosCallback);
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
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        shader.use();

        float timeValue = glfwGetTime();
        shader.setFloat("aspectRatio", (float)SCR_WIDTH/(float)SCR_HEIGHT);
        shader.setFloat("time", timeValue);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        shader.setVec2("mousePos", (float)(mouseX/SCR_WIDTH), 1.0f - (float)(mouseY/SCR_HEIGHT));

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
