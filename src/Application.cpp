#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "Camera.h"
#include "Lighting.h"
#include "Shader.h"
#include "Texture.h"

bool firstMouse = true;
double lastxpos, lastypos;
Camera cam[2] = { Camera(), Camera() };
int selectedCamera = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() { 
	//Initializing GLFW

	if (!glfwInit()) {
		return -1;
	}

	//Setting up OpenGL Core Profile + MSAA

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating window and checking initialization

	GLFWwindow* window = glfwCreateWindow(1600, 900, "Kitten", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Initializing GLEW

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	//Enabling MSAA and Depth Testing

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	 {
		//Raw Data

		//FLOOR

		std::vector<float> data = {
			//POSITION             //NORMAL              //TEXCOORD      //COLOR             //TEXINDEX
			-0.5f,  0.0f,  0.5f,   0.0f,  1.0f,  0.0f,    0.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f,
			 0.5f,  0.0f,  0.5f,   0.0f,  1.0f,  0.0f,   30.0f,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f,
			 0.5f,  0.0f, -0.5f,   0.0f,  1.0f,  0.0f,   30.0f, 30.0f,   1.0f, 1.0f, 1.0f,   0.0f,
			-0.5f,  0.0f, -0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 30.0f,   1.0f, 1.0f, 1.0f,   0.0f, 
		};			

		std::vector<unsigned int> indices = {
			0, 1, 2, 2, 3, 0
		};

		std::vector<Texture> tex = {
			Texture("diffuse", "assets/textures/grass.jpg", GL_LINEAR, GL_REPEAT),
			Texture("specular", "assets/textures/nospec.jpg", GL_LINEAR, GL_REPEAT)
		};

		//BOXES

		std::vector<float> box = {
			 //POSITION           //NORMAL             //TEXCOORD   //COLOR            //TEXINDEX
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
								 		 	    
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
								 		 	    
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
								 		 	    
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
								 			    
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
								 			    
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  0.0f
		};

		std::vector<Texture> boxtex = {
			Texture("diffuse", "assets/textures/box.png", GL_LINEAR, GL_REPEAT),
			Texture("specular", "assets/textures/box_specular.png", GL_LINEAR, GL_REPEAT)
		};

		std::vector<unsigned int> boxindices = {
			0, 1, 2, 2, 3, 0,
			4, 7, 6, 6, 5, 4,
			8, 9, 10, 10, 11, 8, 
			12, 15, 14, 14, 13, 12,
			16, 17, 18, 18, 19, 16, 
			20, 23, 22, 22, 21, 20
		};

		glm::vec3 positions[] = {
			glm::vec3(0.0f,  0.5f, 2.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, 2.2f, -2.5f),
			glm::vec3(-3.8f, 2.0f, -12.3f),
			glm::vec3(2.4f, 0.5f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(4.0f, 2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.5f,  1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		//HUD

		std::vector<float> hudpos = {
			//POSITION        //TEXCOORD    //TEXINDEX
			-0.05f, -0.05f,   0.0f, 0.0f,   0.0f,
			 0.05f, -0.05f,   1.0f, 0.0f,   0.0f,
			 0.05f,  0.05f,   1.0f, 1.0f,   0.0f,
			-0.05f,  0.05f,   0.0f, 1.0f,   0.0f
		};

		std::vector<unsigned int> hudindices = {
			0, 1, 2, 2, 3, 0
		};

		std::vector<Texture> hudtex = {
			Texture("hud", "assets/textures/crosshair.png", GL_LINEAR, GL_CLAMP_TO_EDGE)
		};

		std::vector<float> lamp = {
			//POSITION            //TEXCOORD   //TEXINDEX
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,

			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f
		};

		std::vector<unsigned int> lampindices = {
			0, 1, 2, 2, 3, 0,
			4, 7, 6, 6, 5, 4,
			8, 9, 10, 10, 11, 8,
			12, 15, 14, 14, 13, 12,
			16, 17, 18, 18, 19, 16,
			20, 23, 22, 22, 21, 20
		};

		std::vector<Texture> lamptex = {
			Texture("diffuse", "assets/textures/sea_lantern.png", GL_NEAREST, GL_CLAMP_TO_EDGE),
		};

		//Creating Renderer and Camera objects (not singleton)

		Renderer renderer;

		//HUD Attributes
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);
		Mesh hud(hudpos, hudindices, hudtex, layout);
		layout.Clear();

		//HUD Shader

		glm::mat4 orthoMat = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);

		Shader hudshader("assets/shaders/hud.shader");
		hudshader.EnableBlending();
		hudshader.SetUniformMat4f("orthoMat", orthoMat);

		//FLOOR Attributes

		Mesh floor(data, indices, tex, 32.0f);
		glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 0.5f, 100.0f));
		
		//BOX
		
		Mesh boxmesh(box, boxindices, boxtex, 32.0); 
		glm::mat4 boxModel(1.0f); 

		//Global Shader

		Shader basic("assets/shaders/basic.shader");
		basic.EnableBlending();
		 
		//Setting Lighting Attributes

		Shader lampShader("assets/shaders/lightcube.shader");

		std::vector<glm::vec3> pointLights = {
			//POSITION					  //COLOR						//BRIGHTNESS
			glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec3(1.0f),
			glm::vec3( 5.0f, 1.0f,-6.0f), glm::vec3( 1.0f, 0.0f, 1.0f), glm::vec3(1.0f),
			glm::vec3(-6.0f, 1.0f, 3.0f), glm::vec3( 0.0f, 1.0f, 1.0f), glm::vec3(1.0f),
			glm::vec3(-6.0f, 1.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f)
		};

		
		//Directional Lighting
		
		DirLight directional(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f), 0.2);
		directional.updateUniforms(basic);

		//Point Lights

		PointLightList pointList;
		pointList.PushList(pointLights);
		pointList.updateUniforms(basic);

		//Spot Light
		SpotLightList spotList;
		spotList.Push(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.8f, 0.7f, 0.5f), 1.0f);
		spotList.updateUniforms(basic);
				
		Texture T7("diffuse", "assets/textures/flashlight.png", GL_LINEAR, GL_CLAMP_TO_BORDER);
		T7.Bind(7);

		//Point Light Model

		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		Mesh pLight(lamp, lampindices, lamptex, layout);
		layout.Clear();

		//Rendering Scene
	
		float k = 0.0f;
		float currentFrame = 0.0f;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

		while (!glfwWindowShouldClose(window)) {
			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			renderer.Clear(0.1f, 0.1f, 0.2f);

			//GET MOVEMENT
			cam[selectedCamera].CameraUpdate(window, deltaTime, basic);

			//DRAW FLOOR

			basic.SetModel("u_Model", "u_Normal", modelMatrix);
			floor.Draw(basic);

			//DRAW BOXES

			for (int i = 0; i < 10; i++) {
				boxModel = glm::translate(glm::mat4(1.0f), positions[i]);
				basic.SetModel("u_Model", "u_Normal", boxModel);
				boxmesh.Draw(basic);
			}
		
			//DRAW POINT LIGHTS
			pointList.list[0].setPosition(glm::vec3(5 * glm::sin(glm::radians(k)), 1.0f, 0.0f));
			pointList.list[1].setPosition(glm::vec3(5 * glm::sin(glm::radians(k + 90.0f)), 1.0f, -5.0f));
			pointList.list[2].setPosition(glm::vec3(5 * glm::sin(glm::radians(k + 180.0f)), 1.0f, 3.0f));
			pointList.updateUniforms(basic);

			pointList.Draw(pLight, lampShader, cam[selectedCamera].getView(), cam[selectedCamera].getProjection());

			//UPDATE FLASHLIGHT
			spotList.updateUniforms(basic);

			//DRAW HUD
			hud.Draw(hudshader);

			glfwSwapBuffers(window);
			glfwPollEvents();

			k += 0.25f;
			if (k >= 360.0f) {
				k -= 360.0f;
			}
		}
	}

	glfwTerminate();
	return 0;
}

//Callback functions

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastxpos = xpos;
		lastypos = ypos;
		firstMouse = false;
	}

	cam[selectedCamera].MouseUpdate(xpos - lastxpos, ypos - lastypos);

	lastxpos = xpos;
	lastypos = ypos;
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		selectedCamera++;
		if (selectedCamera == 2) {
			selectedCamera = 0;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	cam[selectedCamera].ScrollUpdate(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		cam[selectedCamera].flashlightOn++;
		if (cam[selectedCamera].flashlightOn == 2) {
			cam[selectedCamera].flashlightOn = 0;
		}
	}
}
