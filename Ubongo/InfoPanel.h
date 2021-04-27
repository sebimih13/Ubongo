#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "SpriteRenderer.h"

class InfoPanel
{
public:
	// constructor
	InfoPanel();

	void Draw();

private:
	std::vector<int> Game[7];

	SpriteRenderer* RenderSprite;
	void DrawPiece(int index, glm::vec2 position);

	float DiffX, DiffY;
};

