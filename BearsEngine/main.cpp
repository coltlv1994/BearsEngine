#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Screen ettings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "init fail on GLFW." << std::endl;
		return -1;
	}
	else
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE); // for HiDPI monitors
	}

	GLFWwindow* m_mainWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Main Window", NULL, NULL);
	if (!m_mainWindow)
	{
		std::cout << "Cannot open main window." << std::endl;
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(m_mainWindow);
	glfwSetFramebufferSizeCallback(m_mainWindow, framebuffer_size_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_mainWindow, true);
	ImGui_ImplOpenGL3_Init();

	// HiDPI awareness
	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -3;
	}

	// Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	// Lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	// Sphere shader
	Shader lightingShader("ShaderCode\\sphere.vs", "ShaderCode\\sphere.fs");
	lightingShader.Use();

	// Spherical mesh grid
	//MeshGrid firstSphere = MeshGrid("vertices.txt", "triangles.txt", "Textures\\earth.jpg");
	MeshGrid firstSphere = MeshGrid(50, 50, "Textures\\earth.jpg");

	// Prospective projection handling
	float zNear = 0.1f;
	float zFar = 100.0f;
	const float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

	// Don't render "rear" faces
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// Frametime
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Rotation
	float theta_Y_in_degree = 0.0f;

	while (!glfwWindowShouldClose(m_mainWindow))
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Control");
		ImGui::Text("Camera Control");

		if (ImGui::Button("Forward"))
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}

		if (ImGui::Button("Back"))
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}

		if (ImGui::Button("Left"))
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}

		if (ImGui::Button("Right"))
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}

		ImGui::Text("Sphere Control");
		if (ImGui::Button("Rotate counterclockwise"))
		{
			theta_Y_in_degree += 5.0f;
		}

		if (ImGui::Button("Rotate clockwise"))
		{
			theta_Y_in_degree -= 5.0f;
		}

		ImGui::End();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader properties
		lightingShader.Use();
		lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.SetVec3("lightPos", lightPos);
		lightingShader.SetVec3("viewPos", camera.Position);

		// view/prospective projection transformations
		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom), aspectRatio, zNear, zFar);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.SetMat4("projection", projection);
		lightingShader.SetMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(theta_Y_in_degree), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.SetMat4("model", model);
		glm::mat3 model_3 = glm::mat3(model); // get the upper left part
		model_3 = glm::transpose(glm::inverse(model_3));
		lightingShader.SetMat3("t_i_model", model_3);

		// Rendering
		firstSphere.Render(lightingShader);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_mainWindow);

		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}