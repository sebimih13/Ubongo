#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "SpriteRenderer.h"

/*
	
	Table Manager contains:
		- 1 table
		- 4 moveable pieces
	
*/

class TableManager
{
public:
	// contructor
	TableManager();

	void Draw();

	void SetMouseLeft(bool pressed);
	void SetMousePos(int x, int y);

private:
	// Render data
	GLuint LineVAO, SquareVAO;
	void InitRenderData();

	SpriteRenderer* RenderSprite;

	// mouse position
	int MouseX, MouseY, MouseDiffX, MouseDiffY;

	// table
	std::vector<std::vector<int>> Table;
	int TableUpX, TableUpY;
	int TableRows, TableColumns;
	int SquareSize;
	float Resize;

	// 4 moveable pieces
	int SelectedPiece;

	int MoveablePieces[5];
	glm::vec2 PiecePosition[5];

	void DrawPiece(int index);

	// todo : add functions to flip and rotate the pieces
	void RotatePiece();
	void FlipPiece();

	void SetSelectedPiece();
	bool IsPieceSelected(int index);
	void DeletePieceFromTable(int index);
	void PutInTable();
};

