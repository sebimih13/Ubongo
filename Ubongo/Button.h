#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TextRenderer.h"
#include "SpriteRenderer.h"

#include <iostream>
#include <string>

class Button
{
public:
	// constructor
	Button(glm::vec2 position, glm::vec2 size, glm::vec3 color);

	void ProcessInput(int xpos, int ypos);
	void SetLeftMouse(bool press);

	void RenderWithText(TextRenderer* TextRender, std::string text, glm::vec2 textOffset);
	void RenderWithSprite(SpriteRenderer* RenderSprite, float rotate);

	bool IsClicked();

private:
	// button state
	glm::vec2 Position, Size;
	glm::vec3 MainColor, SecondColor;

	bool MouseInButtonRange;
	bool LeftMousePressed;

	// render data
	GLuint SquareVAO;
	void InitRenderData();
};

