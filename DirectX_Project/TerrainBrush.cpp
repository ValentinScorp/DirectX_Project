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
	posX = x;
}

void TerrainBrush::SetY(float y)
{
	posY = y;
}
