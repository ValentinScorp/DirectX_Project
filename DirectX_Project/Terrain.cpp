#include "Terrain.h"

Terrain::Terrain(int w, int h, float t)
{
	width = w;
	height = h;
	tile = t;
}

Terrain::~Terrain()
{
}

void Terrain::CreateGraphics(TerrainRenderer * tr)
{
	tr->Create(width, height, tile);
	terrainRenderer = tr;

}

TerrainRenderer * Terrain::GetTerrainRenderer()
{
	return terrainRenderer;
}

void Terrain::Clear()
{
}

