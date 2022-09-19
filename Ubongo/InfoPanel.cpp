#include "InfoPanel.h"
#include "ResourceManager.h"

InfoPanel::InfoPanel(TableManager* Table)
{
	// configure sprite renderer
	RenderSprite = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	// configure table
	this->Table = Table;

	// diff between pieces
	DiffX = 60.0f, DiffY = 70.0f;

	// configure panel boxes
	BoxInfo[0] = 40;
	BoxInfo[1] = 125;
	BoxInfo[2] = 200;
	BoxInfo[3] = 255;
	BoxInfo[4] = 330;
	BoxInfo[5] = 390;
	BoxInfo[6] = 460;

	// prepare each set
	for (int i = 1; i <= 6; i++)
		Game[i].resize(5, 0);
}

void InfoPanel::DrawPiece(int index, glm::vec2 position)
{
	int SquareSize = (int)(ResourceManager::GetTexture("square").Width * 0.1f);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (ResourceManager::PiecesFormat[index][4 * i + j] == '*')
				RenderSprite->DrawSprite(ResourceManager::GetTexture("square"), glm::vec2(position.x + j * SquareSize, position.y), 0.1f, ResourceManager::PiecesColor[index]);
		}
		position.y += SquareSize;
	}
}

void InfoPanel::Draw()
{
	// draw panel
	RenderSprite->DrawSprite(ResourceManager::GetTexture("panel"), glm::vec2(30.0f, 30.0f));

	// draw panel's pieces	
	float StartX = 80.0f, StartY = 65.0f;
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			DrawPiece(Game[i][j], glm::vec2(StartX + j * DiffX, StartY));
		}
		StartY += DiffY;
	}
}

void InfoPanel::SetPanelInfo(int set, int piece, int value)
{
	Game[set][piece] = value;
	Table->SetPieces(Game[set]);
}

void InfoPanel::SetMouseLeft(bool pressed)
{
	if (pressed)
		CheckBoxes();
}

void InfoPanel::SetMousePos(int x, int y)
{
	MouseX = x;
	MouseY = y;
}

void InfoPanel::CheckBoxes()
{
	for (int i = 1; i <= 6; i++)
	{
		if (40 <= MouseX && MouseX <= 110 && BoxInfo[i - 1] <= MouseY && MouseY <= BoxInfo[i])
		{
			Table->ClearTable();
			Table->SetPieces(Game[i]);
		}
	}
}

