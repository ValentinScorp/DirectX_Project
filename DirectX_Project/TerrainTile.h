#pragma once

#include <vector>

#include "Camera.h"

struct TerrainPoint {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 textureCo;
	D3DXVECTOR2 alphaCo;
};

class TerrainTile
{
private:	

	struct Triangle {
		TerrainPoint A;
		TerrainPoint B;
		TerrainPoint C;
	};

	std::vector <TerrainPoint> points;

	Triangle triangle1;
	Triangle triangle2;
public:
	TerrainTile(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3, D3DXVECTOR3 point4);
	~TerrainTile();

	bool Intersection(RayVector ray, D3DXVECTOR3 &intersectionVertex);

	std::vector <TerrainPoint> & GetPoints();
	void ClearPoints();

private:
	bool intersectRayTriangle(RayVector ray, Triangle triangle, D3DXVECTOR3 &intersectionVertex);
};

