#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

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

	void SetBlocks(int row, int column);
	void SetPieces(std::vector<int>& v);

	void SetMouseLeft(bool pressed);
	void SetMousePos(int x, int y);

	void RotatePiece(bool right);
	void FlipPiece();

	bool IsSolutionValid();

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
	std::string PieceFormat[5];

	void DrawPiece(int index);

	void SetSelectedPiece();
	bool IsPieceSelected(int index);
	void DeletePieceFromTable(int index);
	void PutInTable();
};

