#include<glad/gl.h>
#include<GLFW/glfw3.h>

#include<ImGui/imgui.h>
#include<ImGui/backends/imgui_impl_glfw.h>
#include<ImGui/backends/imgui_impl_opengl3.h>
#include<string>
#include<iostream>
#include"game/game.h"

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

Game BreakOut(WINDOW_WIDTH, WINDOW_HEIGHT);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



int main() {

	float angle = 15.0f;
	std::string angleF = std::to_string(angle);

	if (!glfwInit())
		return 1;
	const char* glsl_version = "#version 330 core";
	//setting the version of opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window with graphics context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "This is a own config of dear ImGui example", nullptr, nullptr);
	
	if (window == nullptr)
		return 1;

	glfwMakeContextCurrent(window);
	
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	//glfwSwapInterval(1); // enable vsync
	
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::vec4 clear_color(0.45f, 0.55f, 0.60f, 1.00f);
	//Setting up ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable KeyBoard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  //Enable Gamepad Controls

	//Setting style of imGui
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui_ImplOpenGL3_Init(glsl_version);

	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	BreakOut.init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();


		
		//// Start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		
		BreakOut.renderGUI();
		BreakOut.processInput(deltaTime);
		BreakOut.update(deltaTime, window);
		
		// Rendering
		ImGui::Render();


		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//Rendering game
		BreakOut.render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	

	if (key >= 0 && key < 1024) {
		

		if (action == GLFW_PRESS)
			BreakOut.keys[key] = true;
		else if (action == GLFW_RELEASE) {
			BreakOut.keys[key] = false;
			BreakOut.keysProcessed[key] = false;
		}
	}
}