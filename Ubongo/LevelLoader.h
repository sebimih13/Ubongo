#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "InfoPanel.h"
#include "Table.h"

class Level
{
public:
	// constructor
	Level(InfoPanel* Panel, TableManager* Table);

	void Load(int index);

private:
	InfoPanel* Panel;
	TableManager* Table;
};

