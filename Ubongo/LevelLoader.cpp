#include "LevelLoader.h"

Level::Level(InfoPanel* Panel, TableManager* Table)
{
	this->Panel = Panel;
	this->Table = Table;
}

void Level::Load(int index)
{
	// clear old data
	Table->ClearBlocks();

	// load from file
	std::ifstream fstream("levels/" + std::to_string(index) + ".lvl");

	if (fstream)
	{
		char c;
		for (int i = 1; i <= 6; i++)
		{
			for (int j = 1; j <= 5; j++)
			{
				fstream >> c;
				if (c == '0')
					Table->SetBlocks(i, j);
			}
		}

		int nr;
		for (int i = 1; i <= 6; i++)
		{
			for (int j = 1; j <= 4; j++)
			{
				fstream >> nr;
				Panel->SetPanelInfo(i, j, nr);
			}
		}
	}
}

