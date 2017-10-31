#include "TerrainBrush.h"

TerrainBrush::TerrainBrush()
{
}


TerrainBrush::~TerrainBrush()
{
}

float TerrainBrush::GetWidth()
{
	return width;
}

float TerrainBrush::GetX()
{
	return posX;
}

float TerrainBrush::GetY()
{
	return posY;
}

void TerrainBrush::SetX(float x)
{
	int w = width;
	int newx = (((int)x + w / 2) / w) * w;

	posX = newx;
}

void TerrainBrush::SetY(float y)
{
	int w = width;
	int newy = (((int)y + w / 2) / w) * w;
	posY = newy;
}
