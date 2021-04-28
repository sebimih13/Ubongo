#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "SpriteRenderer.h"
#include "Table.h"

class InfoPanel
{
public:
	// constructor
	InfoPanel(TableManager* Table);

	void Draw();

	void SetPanelInfo(int set, int piece, int value);

	void SetMouseLeft(bool pressed);
	void SetMousePos(int x, int y);

private:
	// mouse position
	int MouseX, MouseY;

	// sprite renderer
	SpriteRenderer* RenderSprite;
	void DrawPiece(int index, glm::vec2 position);

	// panel info
	std::vector<int> Game[7];

	// check panel box
	void CheckBoxs();
	int BoxInfo[7];

	// diff between pieces
	float DiffX, DiffY;

	// table
	TableManager* Table;
};

