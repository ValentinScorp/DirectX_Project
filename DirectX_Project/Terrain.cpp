#include "Terrain.h"
#include "TerrainRenderer.h"

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
	tr->CreateTexture("sand.png");
	tr->CreateTexture("grass.png");
	tr->CreateTexture("rock.png");
	tr->CreateTexture("alphaSide.png");
	tr->CreateTexture("alphaCorner.png");
	tr->CreateTexture("alphaFull.png");
	tr->CreateTexture("alphaDiag.png");
		
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			TerrainPatch patch(i, j, patchDimention, tile);
			std::vector<TerrainTile> ptiles = patch.GetTiles();			
			tiles.insert(tiles.end(), ptiles.begin(), ptiles.end());
		}
	}

	std::vector <TerrainPoint> tp;

	for (auto t : tiles) {
		auto points = t.GetPoints();
		for (auto p : points) {
			tp.push_back(p);
		}
		t.ClearPoints();
	}

	tr->Create(tp);
}

D3DXVECTOR3 Terrain::GetTerraneIntersection(RayVector rv)
{
	D3DXVECTOR3 intersection = { 0.0f, 0.0f, 0.0f };

	for (auto t : tiles) {
		if (t.Intersection(rv, intersection)) {
			return intersection;
		}
	}
	
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

