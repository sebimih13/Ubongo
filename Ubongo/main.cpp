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

// callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// utility functions
void Init();
void processInput(GLFWwindow* window);

// window settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// time variables
double deltaTime = 0.0f;		// time between current frame and last frame
double lastFrame = 0.0f;		// time of last frame

// mouse variables
double MouseX, MouseY;

// table
std::vector<std::vector<int>> Table;

void DrawTable(float size);

// panel and pieces
std::string PiecesFormat[15];
glm::vec3 PiecesColor[15];

void DrawPanelPieces();
void DrawPiece(int index, glm::vec2 position, float size);

// Sprite Renderer
SpriteRenderer* RenderSprite;

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


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// input
		processInput(window);


		// rendering commands here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawTable(0.6f);
		DrawPanelPieces();


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete RenderSprite;


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
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// todo
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Utility functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint SquareVAO, LineVAO;
void Init()
{
	// SQUARE
	float SquareVertices[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	GLuint SquareVBO;
	glGenVertexArrays(1, &SquareVAO);
	glGenBuffers(1, &SquareVBO);

	glBindVertexArray(SquareVAO);

	glBindBuffer(GL_ARRAY_BUFFER, SquareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVertices), SquareVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// LINE
	float LineVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	GLuint LineVBO;
	glGenVertexArrays(1, &LineVAO);
	glGenBuffers(1, &LineVBO);

	glBindVertexArray(LineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, LineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineVertices), LineVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &LineVBO);
	glDeleteBuffers(1, &SquareVBO);

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

	// configure renderer
	RenderSprite = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	/*
		Pieces format: 3x3
		
		1 : **###		2:	**###		3:	**###		4:	*####		5:	****#		6:	****#	
			#*###			*####			**###			*####			#####			##*##			
			#**##			*####			#####			#####			#####			#####			
			#####			#####			#####			#####			#####			#####			
			#####			#####			#####			#####			#####			#####			
			#####			#####			#####			#####			#####			#####			
		
		7:  ***##		8:	#**##		9:	***##		10:	****#		11:	***##		12:	**###		
			#**##			**###			#####			###*#			#*###			*####			
			#####			#####			#####			#####			#####			#####		
			#####			#####			#####			#####			#####			#####		
			#####			#####			#####			#####			#####			#####			
			#####			#####			#####			#####			#####			#####	

	*/

	// todo : make 12 texture for pieces + colors
	PiecesFormat[1] = "**####*####**##";
	PiecesFormat[2] = "**###*####*####";
	PiecesFormat[3] = "**###**########";
	PiecesFormat[4] = "*####*#########";
	PiecesFormat[5] = "****###########";
	PiecesFormat[6] = "****###*#######";
	PiecesFormat[7] = "***###**#######";
	PiecesFormat[8] = "#**##**########";
	PiecesFormat[9] = "***############";
	PiecesFormat[10] = "****####*######";
	PiecesFormat[11] = "***###*########";
	PiecesFormat[12] = "**###*#########";

	PiecesColor[1] = glm::vec3(1.0f, 1.0f, 0.0f);
	PiecesColor[2] = glm::vec3(0.0f, 0.4f, 0.7f);
	PiecesColor[3] = glm::vec3(0.1f, 0.9f, 0.5f);
	PiecesColor[4] = glm::vec3(0.8f, 0.0f, 0.7f);
	PiecesColor[5] = glm::vec3(0.8f, 0.5f, 0.0f);
	PiecesColor[6] = glm::vec3(0.0f, 0.3f, 0.1f);
	PiecesColor[7] = glm::vec3(0.7f, 0.0f, 0.2f);
	PiecesColor[8] = glm::vec3(0.7f, 0.6f, 0.5f);
	PiecesColor[9] = glm::vec3(0.8f, 0.8f, 1.0f);
	PiecesColor[10] = glm::vec3(1.0f, 1.0f, 1.0f);
	PiecesColor[11] = glm::vec3(0.4f, 0.1f, 0.5f);
	PiecesColor[12] = glm::vec3(0.1f, 1.0f, 0.8f);

	// configure table 6x5
	Table.resize(7, std::vector<int>(6, 0));
}

void processInput(GLFWwindow* window)
{
	// todo
	glfwGetCursorPos(window, &MouseX, &MouseY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Table 6x5
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawTable(float size)
{
	int SquareSize = ResourceManager::GetTexture("square").Width * size;
	int TableUpX = 600;
	int TableUpY = 30;

	// todo : draw pieces in table
	// todo : for loop in Table[][]
	DrawPiece(2, glm::vec2(TableUpX, TableUpY), size);

	// draw rows and columns
	int TotalRows = 6;
	int TotalColumns = 5;

	glm::mat4 model = glm::mat4(1.0f);

	ResourceManager::GetShader("line").Use();
	ResourceManager::GetShader("line").SetVector3f("color", glm::vec3(0.9f, 0.9f, 0.9f));

	glBindVertexArray(LineVAO);
	glLineWidth(2.0f);

	// draw rows
	for (int y = TableUpY; y <= TableUpY + TotalRows * SquareSize; y += SquareSize)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TableUpX, y, 0.0f));
		model = glm::scale(model, glm::vec3(TotalColumns * SquareSize, 0.0f, 0.0f));
		ResourceManager::GetShader("line").SetMatrix4f("model", model);

		glDrawArrays(GL_LINES, 0, 2);
	}

	// draw columns
	for (int x = TableUpX; x <= TableUpX + TotalColumns * SquareSize; x += SquareSize)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, TableUpY, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(TotalRows * SquareSize, 0.0f, 0.0f));
		ResourceManager::GetShader("line").SetMatrix4f("model", model);

		glDrawArrays(GL_LINES, 0, 2);
	}

	glLineWidth(1.0f);
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Panel + Pieces
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawPiece(int index, glm::vec2 position, float size)
{
	int SquareWidth = ResourceManager::GetTexture("square").Width * size;
	int SquareHeight = ResourceManager::GetTexture("square").Height * size;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (PiecesFormat[index][5 * i + j] == '*')
				RenderSprite->DrawSprite(ResourceManager::GetTexture("square"), glm::vec2(position.x + j * SquareWidth, position.y), size, PiecesColor[index]);
		}
		position.y += SquareHeight;
	}
}

void DrawPanelPieces()
{
	// draw panel
	RenderSprite->DrawSprite(ResourceManager::GetTexture("panel"), glm::vec2(30.0f, 30.0f));

	// draw panel's pieces
	float resize = 0.1f;
	float dif = 70.0f, start = 65.0f;
	DrawPiece(1, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(2, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(3, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(4, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(5, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(6, glm::vec2(130.0f, start), resize);		start += dif;

	// todo : delete
	DrawPiece(7, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(8, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(9, glm::vec2(130.0f, start), resize);		start += dif;
	DrawPiece(10, glm::vec2(130.0f, start), resize);	start += dif;
	DrawPiece(11, glm::vec2(130.0f, start), resize);	start += dif;
	DrawPiece(12, glm::vec2(130.0f, start), resize);	start += dif;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//														Moveable Pieces
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// todo : add functions to flip and rotate the pieces

