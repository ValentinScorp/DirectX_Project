#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int y, int tilesDim, float tileSize)
{
	float xPos = x * tileSize;
	float yPos = y * tileSize;

	for (int i = 0; i < tilesDim; i++) {
		for (int j = 0; j < tilesDim; j++) {
			D3DXVECTOR3	tp1(xPos + j * tileSize, yPos + i * tileSize, 0.0f);
			D3DXVECTOR3	tp2(xPos + j * tileSize, yPos + (i + 1) * tileSize, 0.0f);
			D3DXVECTOR3	tp3(xPos + (j + 1) * tileSize, yPos + (i + 1) * tileSize, 0.0f);
			D3DXVECTOR3	tp4(xPos + (j + 1) * tileSize, yPos + i * tileSize, 0.0f);

			TerrainTile t(tp1, tp2, tp3, tp4);
			tiles.push_back(t);
		}
	}
}

TerrainPatch::~TerrainPatch()
{
}

std::vector<TerrainTile>& TerrainPatch::GetTiles()
{
	return tiles;
}
