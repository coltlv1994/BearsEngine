#include <iostream>
#include "GLFW\glfw3.h"

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "init fail on GLFW." << std::endl;
		return -1;
	}

	GLFWwindow* m_mainWindow = glfwCreateWindow(1200, 900, "Main Window", NULL, NULL);
	if (!m_mainWindow)
	{
		std::cout << "Cannot open main window." << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_mainWindow);

	while (!glfwWindowShouldClose(m_mainWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering

		glfwSwapBuffers(m_mainWindow);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}