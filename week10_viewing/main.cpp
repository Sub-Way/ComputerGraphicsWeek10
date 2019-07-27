#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GL\GLU.h>
#include <GL\freeglut.h>

#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include "math.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include "Geometry/TriangularSurface/StaticTriangularSurface.h"
#include "GL2/GL2_Object.h"
#include "GL2/GL2_World.h"
#include "GL2/GL2_Light.h"
#include "GL2/GL2_Material.h"

int width_window = 640;
int height_window = 480;

GL2_World gl_world;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	gl_world.camera_.UpdateDolly(yoffset); // zoom in & zoom out => dolly
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) //마우스 오른쪽 버튼 눌렸을 때
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); //x와 y좌표 값 받아온다.

		gl_world.camera_.StartMouseRotation(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) //마우스 오른쪽 버튼 떼었을 때
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		gl_world.camera_.EndMouseRotation(xpos, ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) //마우스 왼쪽 버튼 눌렀을 때
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		gl_world.camera_.StartMousePan(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) //마우스 왼쪽 버튼 떼었을 때
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		gl_world.camera_.EndMousePan(xpos, ypos);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) //계속적으로 마우스의 위치를 받아온다.
{
	//std::cout << xpos << " " << ypos << std::endl;

	gl_world.camera_.ProcessMouseMotion(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		static int proj = 1;
		gl_world.camera_.SetMode(proj);
	}

	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		static int proj = -1;
		gl_world.camera_.SetMode(proj);
	}

	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		static int proj = -1;
		gl_world.camera_.SetMode_2(proj);
	}

	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		static int proj = 1;
		gl_world.camera_.SetMode_2(proj);
	}

	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		static float proj = -0.1;
		glm::vec3 c(0.0, 1.0, 0.0);
		gl_world.camera_.SetMode_3(proj, c);
	}

	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		static float proj = 0.1;
		glm::vec3 c(0.0, 1.0, 0.0);
		gl_world.camera_.SetMode_3(proj, c);
	}
}


int main(void)
{
	GLFWwindow *window = nullptr;

	/* Initialize the library */
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 32);

	// window resolution
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width_window = mode->width * 0.8f;
	height_window = mode->height * 0.8f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width_window, height_window, "Hello World", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callbacks here
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(1, 1, 1, 1); // while background

	printf("%s\n", glGetString(GL_VERSION));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	const float aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glOrtho(-1.2*aspect, 1.2*aspect, -1.2, 1.2, -100.0, 100.0);
	gluLookAt(0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0, 1, 0);//camera takes care of it

	const float zNear = 0.001, zFar = 100.0, fov = 45.0;			// UI
	gl_world.camera_.Resize(width, height, fov, zNear, zFar);
	gl_world.initShaders();

	glEnable(GL_MULTISAMPLE);

	StaticTriangularSurface surface;
	//surface.readObj("BuddhaSculpture.obj", true, true);
	surface.readObj("Box.obj", true, true); // 파일명, object의 좌표값을 -1~1사이로 만들기, 원점으로 이동

	GL2_Object gl_obj; //shadding을 해준다.
	gl_obj.initPhongSurface(surface);
	//gl_obj.mat_.setRed();
	gl_obj.mat_.setGold();

	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);

	glLoadIdentity();

	GL2_Light light;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		gl_world.camera_.ContinueRotation();

		glm::mat4 vp = gl_world.camera_.GetWorldViewMatrix();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gl_world.shaders_.program_id_);

		gl_world.shaders_.sendUniform(vp, "mvp");

		gl_obj.applyLighting(light);
		gl_obj.drawWithShader(gl_world.shaders_);

		glUseProgram(0);

		// Old-style rendering

		glLoadMatrixf(&vp[0][0]);

		glLineWidth(2);
		gl_world.drawAxes();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	glfwTerminate();

	return 0;
}

