#include "Table.h"
#include "ResourceManager.h"

TableManager::TableManager()
{
	// mouse
	MouseX = MouseY = MouseDiffX = MouseDiffY = 0;
	SelectedPiece = 0;

	// table 6x5
	Table.resize(7, std::vector<int>(6, 0));

	// todo : get data for block squares
	Table[4][4] = -1;

	TableUpX = 600;
	TableUpY = 30;

	TableRows = 6;
	TableColumns = 5;

	Resize = 0.5f;
	SquareSize = ResourceManager::GetTexture("square").Width * Resize;

	// 4 moveable pieces
	PiecePosition[1] = glm::vec2(210.0f, 490.0f);
	PiecePosition[2] = glm::vec2(210.0f + 5.0f * SquareSize, 490.0f);
	PiecePosition[3] = glm::vec2(210.0f, 490.0f + 3 * SquareSize + 10.0f);
	PiecePosition[4] = glm::vec2(210.0f + 5.0f * SquareSize, 490.0f + 3 * SquareSize + 10.0f);

	//  todo : get data for the 4 moveable pieces
	MoveablePieces[1] = 1;
	MoveablePieces[2] = 10;
	MoveablePieces[3] = 11;
	MoveablePieces[4] = 12;

	// todo : make function to set this vector
	PieceFormat[1] = ResourceManager::PiecesFormat[MoveablePieces[1]];
	PieceFormat[2] = ResourceManager::PiecesFormat[MoveablePieces[2]];
	PieceFormat[3] = ResourceManager::PiecesFormat[MoveablePieces[3]];
	PieceFormat[4] = ResourceManager::PiecesFormat[MoveablePieces[4]];

	InitRenderData();
}

void TableManager::InitRenderData()
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


	// configure sprite renderer
	RenderSprite = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

void TableManager::DrawPiece(int index)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (PieceFormat[index][4 * i + j] == '*')
				RenderSprite->DrawSprite(ResourceManager::GetTexture("square"), glm::vec2(PiecePosition[index].x + j * SquareSize, PiecePosition[index].y + i * SquareSize), Resize, ResourceManager::PiecesColor[MoveablePieces[index]]);
}

void TableManager::Draw()
{
	glm::mat4 model = glm::mat4(1.0f);

	// draw table
	// draw pieces in table
	for (int i = 1; i <= TableRows; i++)
	{
		for (int j = 1; j <= TableColumns; j++)
		{
			int value = Table[i][j];
			if (value > 0)
			{
				RenderSprite->DrawSprite(ResourceManager::GetTexture("square"), glm::vec2(TableUpX + (j - 1) * SquareSize, TableUpY + (i - 1) * SquareSize), Resize, ResourceManager::PiecesColor[value]);
			}
			else if (value == -1)
			{
				ResourceManager::GetShader("line").Use();
				glBindVertexArray(SquareVAO);

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(TableUpX + (j - 1) * SquareSize, TableUpY + (i - 1) * SquareSize, 0.0f));
				model = glm::scale(model, glm::vec3((float)SquareSize, (float)SquareSize, 0.0f));
				ResourceManager::GetShader("line").SetMatrix4f("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glBindVertexArray(0);
			}
		}
	}

	// draw rows and columns
	ResourceManager::GetShader("line").Use();

	glBindVertexArray(LineVAO);
	glLineWidth(2.0f);

	// draw rows
	for (int y = TableUpY; y <= TableUpY + TableRows * SquareSize; y += SquareSize)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TableUpX, y, 0.0f));
		model = glm::scale(model, glm::vec3(TableColumns * SquareSize, 0.0f, 0.0f));
		ResourceManager::GetShader("line").SetMatrix4f("model", model);

		glDrawArrays(GL_LINES, 0, 2);
	}

	// draw columns
	for (int x = TableUpX; x <= TableUpX + TableColumns * SquareSize; x += SquareSize)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, TableUpY, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(TableRows * SquareSize, 0.0f, 0.0f));
		ResourceManager::GetShader("line").SetMatrix4f("model", model);

		glDrawArrays(GL_LINES, 0, 2);
	}

	glLineWidth(1.0f);
	glBindVertexArray(0);

	// draw the moveable pieces
	for (int i = 1; i <= 4; i++)
		DrawPiece(i);
}

void TableManager::RotatePiece(bool right)
{
	if (SelectedPiece == 0)
		return;

	/*
	
		****		###*		####		**##
		###*	=>	###*	=>	####	=>	*###
		####		###*		*###		*###
		####		##**		****		*###


		****		#*##		####		**##
		###*	=>	#*##	=>	####	=>	*###
		####		#*##		*###		*###
		####		**##		****		*###

	*/

	char aux[5][5];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			aux[i][j] = PieceFormat[SelectedPiece][4 * i + j];
		
	// todo : debug output Piece Format
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << aux[i][j];
		}
		std::cout << '\n';
	}
	std::cout << '\n';

	int index = 0;
	if (right)
	{
		// search max row which contains '*'
		int StartRow = 4;
		for (int i = 3; i >= 0 && StartRow == 4; i--)
			for (int j = 0; j < 4 && StartRow == 4; j++)
				if (aux[i][j] == '*')
					StartRow = i;

		for (int j = 0; j < 4; j++)
		{
			for (int i = StartRow; i >= 0; i--)
				PieceFormat[SelectedPiece][index++] = aux[i][j];

			// fill remaining spaces
			while (index < (j + 1) * 4)
				PieceFormat[SelectedPiece][index++] = '#';
		}
	}
	else
	{
		// search max row which contains '*'
		int StartRow = 4;
		for (int i = 0; i < 4 && StartRow == 4; i++)
			for (int j = 0; j < 4 && StartRow == 4; j++)
				if (aux[i][j] == '*')
					StartRow = i;

		std::cout << StartRow << '\n';

		for (int j = 3; j >= 0; j--)
		{
			for (int i = StartRow; i < 4; i++)
				PieceFormat[SelectedPiece][index++] = aux[i][j];

			// fill remaining spaces
			while (index == 0 || index % 4 != 0)
				PieceFormat[SelectedPiece][index++] = '#';
		}
	}

	// todo : debug output Piece Format
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << PieceFormat[SelectedPiece][4 * i + j];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void TableManager::FlipPiece()
{
	if (SelectedPiece == 0)
		return;

	/*

		****		####
		###*	=>	###*
		####		****	
		####		####

		**##		#**#
		#*##	=>	#*##
		#**#		**##
		####		####

	*/

	// todo : flip
	std::cout << "FLIP\n";


}

bool TableManager::IsPieceSelected(int index)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (PieceFormat[index][4 * i + j] == '*')
			{
				int StartX = (int)PiecePosition[index].x + j * SquareSize;
				int EndX = StartX + SquareSize;

				int StartY = (int)PiecePosition[index].y + i * SquareSize;
				int EndY = StartY + SquareSize;

				if (StartX <= MouseX && MouseX <= EndX && StartY <= MouseY && MouseY <= EndY)
						return true;
			}
		}
	}
	return false;
}

void TableManager::SetSelectedPiece()
{
	for (int i = 1; i <= 4; i++)
	{
		if (IsPieceSelected(i))
		{
			SelectedPiece = i;
			return;
		}
	}
	SelectedPiece = 0;
}

void TableManager::DeletePieceFromTable(int index)
{
	for (int i = 1; i <= TableRows; i++)
		for (int j = 1; j <= TableColumns; j++)
			if (Table[i][j] == index)
				Table[i][j] = 0;
}

void TableManager::PutInTable()
{
	if (SelectedPiece == 0)
		return;

	DeletePieceFromTable(MoveablePieces[SelectedPiece]);

	// todo : identify which square is closer to out position X && Y
	// check if the selected piece must be placed in table

	glm::vec2 CorrectPosition = PiecePosition[SelectedPiece];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (PieceFormat[SelectedPiece][4 * i + j] == '*')
			{
				int StartX = (int)PiecePosition[SelectedPiece].x + j * SquareSize;
				int EndX = StartX + SquareSize;

				int StartY = (int)PiecePosition[SelectedPiece].y + i * SquareSize;
				int EndY = StartY + SquareSize;

				// check if piece is in table
				if (TableUpX <= StartX && EndX <= TableUpX + (TableColumns + 1) * SquareSize
					&& TableUpY <= StartY && EndY <= TableUpY + (TableRows + 1) * SquareSize)
				{
					int Row = (EndY - TableUpY) / SquareSize;
					int Column = (EndX - TableUpX) / SquareSize;
					
					// check if place is already taken
					if (Table[Row][Column] != 0)
					{
						DeletePieceFromTable(MoveablePieces[SelectedPiece]);
						return;
					}

					Table[Row][Column] = MoveablePieces[SelectedPiece];
					CorrectPosition = glm::min(CorrectPosition, glm::vec2(TableUpX + (Column - 1) * SquareSize, TableUpY + (Row - 1) * SquareSize));
				}
				else // piece is outside of table
				{
					DeletePieceFromTable(MoveablePieces[SelectedPiece]);
					return;
				}
			}
		}
	}

	PiecePosition[SelectedPiece] = CorrectPosition;
}

void TableManager::SetMouseLeft(bool pressed)
{
	if (pressed)
	{
		SetSelectedPiece();
	}
	else
	{
		PutInTable();
		SelectedPiece = 0;
	}
}

void TableManager::SetMousePos(int x, int y)
{
	MouseDiffX = x - MouseX;
	MouseDiffY = y - MouseY;

	MouseX = x;
	MouseY = y;
	
	// check if a piece is selected and then move it
	if (SelectedPiece != 0)
	{
		PiecePosition[SelectedPiece].x += MouseDiffX;
		PiecePosition[SelectedPiece].y += MouseDiffY;
	}
}

