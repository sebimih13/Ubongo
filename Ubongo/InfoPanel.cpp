#include "InfoPanel.h"
#include "ResourceManager.h"

InfoPanel::InfoPanel()
{
	// configure sprite renderer
	RenderSprite = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	// todo : make info for Game[][]
	Game[1].push_back(1);
	Game[1].push_back(2);
	Game[1].push_back(3);
	Game[1].push_back(4);

	Game[2].push_back(5);
	Game[2].push_back(6);
	Game[2].push_back(7);
	Game[2].push_back(8);

	Game[3].push_back(9);
	Game[3].push_back(10);
	Game[3].push_back(11);
	Game[3].push_back(12);

	Game[4].push_back(12);
	Game[4].push_back(12);
	Game[4].push_back(12);
	Game[4].push_back(12);

	Game[5].push_back(10);
	Game[5].push_back(11);
	Game[5].push_back(2);
	Game[5].push_back(4);

	Game[6].push_back(5);
	Game[6].push_back(5);
	Game[6].push_back(5);
	Game[6].push_back(5);
}

void InfoPanel::DrawPiece(int index, glm::vec2 position)
{
	int SquareSize = ResourceManager::GetTexture("square").Width * 0.1f;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (ResourceManager::PiecesFormat[index][5 * i + j] == '*')
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
	float StartX = 130.0f, StartY = 65.0f;
	float DiffX = 60.0f, DiffY = 70.0f;

	for (int i = 1; i <= 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			DrawPiece(Game[i][j], glm::vec2(StartX + j * DiffX, StartY));
		}
		StartY += DiffY;
	}
}

