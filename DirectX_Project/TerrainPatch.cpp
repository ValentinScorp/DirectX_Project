#include "TerrainPatch.h"

TerrainPatch::TerrainPatch(int x, int y, int tilesDim, float tileSize)
{
	float xPos = x * tileSize;
	float yPos = y * tileSize;

	float texCoStep = 0;
	if (tilesDim != 0) {
		texCoStep = 1.0f / (float)tilesDim;
	}

	for (int i = 0; i < tilesDim; i++) {
		for (int j = 0; j < tilesDim; j++) {
			
			TerrainPoint tp1, tp2, tp3, tp4;

			tp1.position = { xPos + j * tileSize, yPos + i * tileSize, 0.0f };
			tp2.position = { xPos + j * tileSize, yPos + (i + 1) * tileSize, 0.0f };
			tp3.position = { xPos + (j + 1) * tileSize, yPos + (i + 1) * tileSize, 0.0f };
			tp4.position = { xPos + (j + 1) * tileSize, yPos + i * tileSize, 0.0f };

			tp1.textureCo.x = j * texCoStep;
			tp1.textureCo.y = i * texCoStep;

			tp2.textureCo.x = j * texCoStep;
			tp2.textureCo.y = (i + 1) * texCoStep;

			tp3.textureCo.x = (j + 1) * texCoStep;
			tp3.textureCo.y = (i + 1) * texCoStep;

			tp4.textureCo.x = (j + 1) * texCoStep;
			tp4.textureCo.y = i * texCoStep;					

			TerrainTile t(tp1, tp2, tp3, tp4);

			// убрать потом
			t.SetTextureIndex(0, 1);


			// -------------

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
