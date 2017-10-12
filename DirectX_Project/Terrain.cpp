#include "Terrain.h"

Terrain::Terrain(int w, int h, float t)
{
	width = w;
	height = h;
	patchDimention = 4;
	tile = t;
}

Terrain::~Terrain()
{
}

void Terrain::CreateGraphics(TerrainRenderer * tr)
{

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			TerrainPatch patch(i, j, patchDimention, tile);
			patches.push_back(patch);
		}
	}

	std::vector <TerrainPoint> tp;

	for (auto p : patches) {
		auto tilesInPatch = p.GetTiles();
		for (auto t : tilesInPatch) {
			auto points = t.GetPoints();
			tp.insert(tp.end(), points.begin(), points.end());
			t.ClearPoints();
		}
	}
	tr->Create(tp);
}

TerrainRenderer * Terrain::GetTerrainRenderer()
{
	return terrainRenderer;
}

void Terrain::Clear()
{
}

