#include <iostream>
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "scene.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

unsigned int screenWidth = SCR_WIDTH;
unsigned int screenHeight = SCR_HEIGHT;

const std::string VERT_PATH = "/home/thesynthax/projects/cpp/raytracer/src/vertex.glsl";
const std::string FRAG_PATH = "/home/thesynthax/projects/cpp/raytracer/src/fragment.glsl";

//GLuint screenTexture;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
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
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

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
    /*GLuint FBO;
    glGenTextures(1, &screenTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);*/



    //Shader Initialization and main code loop
    Shader shader(VERT_PATH, FRAG_PATH);
    shader.use();
    initializeUniforms(shader);
    //shader.setInt("u_screenTexture", 0);

    //int accumulatedPasses = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        updateUniforms(shader, window, screenWidth, screenHeight);

        shader.setVec2("u_screenPixels", SCR_WIDTH, SCR_HEIGHT);
        shader.setFloat("u_aspectRatio", (float)SCR_WIDTH/(float)SCR_HEIGHT);
        
        float timeValue = glfwGetTime();
        shader.setFloat("u_time", timeValue);
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        shader.setVec2("u_mousePos", (float)(mouseX/SCR_WIDTH), 1.0f - (float)(mouseY/SCR_HEIGHT));

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) setPoint();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) setSoftShadows();
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) setUseMouseForCamera();

		glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        //shader.setBool("u_directOutputPass", false);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//accumulatedPasses += 1;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//shader.setInt("u_accumulatedPasses", accumulatedPasses);*/
		//shader.setBool("u_directOutputPass", true);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    //Deletion of vertex data and deactivation of shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    //glDeleteFramebuffers(1, &FBO);
	//glDeleteTextures(1, &screenTexture);

    shader.deactivate();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
