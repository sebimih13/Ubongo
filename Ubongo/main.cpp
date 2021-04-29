#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "InfoPanel.h"
#include "Table.h"
#include "LevelLoader.h"
#include "Button.h"

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Utility functions
void Init();
void ProcessInput(GLFWwindow* window);
void CheckSuccess();
void DrawButtons();

// Window settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// Time variables
double deltaTime = 0.0f;		// time between current frame and last frame
double lastFrame = 0.0f;		// time of last frame

// Mouse variables
double MouseX, MouseY;

// Table
TableManager* Table;

// Text/Sprite Renderer
TextRenderer* Text;
SpriteRenderer* Sprite;

// Panel
InfoPanel* Panel;

// Level Loader
int CurrentLevel = 1;
Level* LevelLoader;

// Buttons
Button* NextSceneButton;
Button* PreviousSceneButton;
Button* SolutionButton;

bool SearchingSolution = false;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, false);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ubongo", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global OpenGL state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// initialize buffers and shaders
	Init();

	// load level
	LevelLoader->Load(1);


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// input
		ProcessInput(window);


		// rendering commands here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawButtons();
		Table->Draw();
		Panel->Draw();
		CheckSuccess();


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete Table;
	delete Panel;
	delete LevelLoader;
	delete Text;
	delete Sprite;

	delete NextSceneButton;
	delete PreviousSceneButton;
	delete SolutionButton;

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Callback functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		Table->RotatePiece(true);

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		Table->RotatePiece(false);

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
		Table->FlipPiece();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		Table->SetMouseLeft(true);
		Panel->SetMouseLeft(true);
		NextSceneButton->SetLeftMouse(true);
		PreviousSceneButton->SetLeftMouse(true);
		SolutionButton->SetLeftMouse(true);
		std::cout << "Left Mouse PRESSED\n";
	}
	else
	{
		Table->SetMouseLeft(false);
		Panel->SetMouseLeft(false);
		NextSceneButton->SetLeftMouse(false);
		PreviousSceneButton->SetLeftMouse(false);
		SolutionButton->SetLeftMouse(false);
		std::cout << "Left Mouse RELEASED\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Utility functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Init()
{
	// load shaders
	ResourceManager::LoadShader("shaders/line.vert", "shaders/line.frag", nullptr, "line");
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");

	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f);

	ResourceManager::GetShader("line").Use();
	ResourceManager::GetShader("line").SetMatrix4f("projection", projection);

	ResourceManager::GetShader("sprite").Use();
	ResourceManager::GetShader("sprite").SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4f("projection", projection);

	// load textures
	ResourceManager::LoadTexture("assets/panel.png", true, "panel");
	ResourceManager::LoadTexture("assets/block.png", false, "square");
	ResourceManager::LoadTexture("assets/next.png", true, "next");
	

	// configure resource manager
	ResourceManager::ResourceManager();

	// configure table
	Table = new TableManager();

	// configure panel
	Panel = new InfoPanel(Table);

	// configure level
	LevelLoader = new Level(Panel, Table);

	// configure text/sprite renderer
	Text = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);
	Text->Load("fonts/Antonio-Bold.ttf", 60);

	Sprite = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	
	// buttons
	int TextureSize = ResourceManager::GetTexture("next").Width;
	NextSceneButton = new Button(glm::vec2(250.0f, 650.0f), glm::vec2(TextureSize - 10.0f, TextureSize - 10.0f), glm::vec3(1.0f));
	PreviousSceneButton = new Button(glm::vec2(50.0f, 650.0f), glm::vec2(TextureSize - 10.0f, TextureSize - 10.0f), glm::vec3(1.0f));
	SolutionButton = new Button(glm::vec2(50.0f, 550.0f), glm::vec2(400.0f, 70.0f), glm::vec3(0.5f, 0.5f, 0.5f));
}

void ProcessInput(GLFWwindow* window)
{
	glfwGetCursorPos(window, &MouseX, &MouseY);
	Table->SetMousePos((int)MouseX, (int)MouseY);
	Panel->SetMousePos((int)MouseX, (int)MouseY);
	NextSceneButton->ProcessInput((int)MouseX, (int)MouseY);
	PreviousSceneButton->ProcessInput((int)MouseX, (int)MouseY);
	SolutionButton->ProcessInput((int)MouseX, (int)MouseY);
}

void CheckSuccess()
{
	if (Table->IsSolutionValid())
		Text->RenderText("Success", SCR_WIDTH / 2, SCR_HEIGHT / 2, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void DrawButtons()
{
	NextSceneButton->RenderWithSprite(Sprite, 1.0f);
	PreviousSceneButton->RenderWithSprite(Sprite, 180.0f);
	SolutionButton->RenderWithText(Text, "Show Solution", glm::vec2(35.0f, 10.0f));

	if (NextSceneButton->IsClicked() && !SearchingSolution)
	{
		CurrentLevel++;
		if (CurrentLevel > 9)
			CurrentLevel = 1;
		LevelLoader->Load(CurrentLevel);
	}
	else if (PreviousSceneButton->IsClicked() && !SearchingSolution)
	{
		CurrentLevel--;
		if (CurrentLevel < 1)
			CurrentLevel = 9;
		LevelLoader->Load(CurrentLevel);
	}

	if (SolutionButton->IsClicked())
	{
		SearchingSolution = true;
		Table->MakeSolution();
		SearchingSolution = false;
	}
}

