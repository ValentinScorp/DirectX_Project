#pragma once

#include <vector>

#include "TerrainTile.h"


class TerrainPatch
{
private:
	std::vector <TerrainTile> tiles;

public:
	TerrainPatch(int x, int y, int tilesDim, float tileSize);
	~TerrainPatch();

	std::vector <TerrainTile> & GetTiles();
};

