#include "TerrainTile.h"
/*
2 ---- 3	B ---C			 B
|      |	|  /		  /	 |
1------4	A			A----C
*/
TerrainTile::TerrainTile(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3, D3DXVECTOR3 point4)
{
	triangle1.A.position = point1;
	triangle1.B.position = point3;
	triangle1.C.position = point2;

	triangle2.A.position = point1;
	triangle2.B.position = point4;
	triangle2.C.position = point3;

	triangle2.C.normal.x = 0;
	triangle2.C.normal.y = 0;
	triangle2.C.normal.z = 1;
	triangle1.A.normal = triangle1.B.normal = triangle1.C.normal = triangle2.A.normal = triangle2.B.normal = triangle2.C.normal;

	triangle1.A.textureCo.x = 0; triangle1.A.textureCo.y = 0;
	triangle1.B.textureCo.x = 1; triangle1.B.textureCo.y = 1;
	triangle1.C.textureCo.x = 0; triangle1.C.textureCo.y = 1;

	triangle2.A.textureCo.x = 0; triangle2.A.textureCo.y = 0;
	triangle2.B.textureCo.x = 1; triangle2.B.textureCo.y = 0;
	triangle2.C.textureCo.x = 1; triangle2.C.textureCo.y = 1;

	triangle1.A.alphaCo = triangle1.A.textureCo;
	triangle1.B.alphaCo = triangle1.B.textureCo;
	triangle1.C.alphaCo = triangle1.C.textureCo;

	triangle2.A.alphaCo = triangle2.A.textureCo;
	triangle2.B.alphaCo = triangle2.B.textureCo;
	triangle2.C.alphaCo = triangle2.C.textureCo;
	
}

TerrainTile::TerrainTile(TerrainPoint point1, TerrainPoint point2, TerrainPoint point3, TerrainPoint point4)
{
	triangle1.A.position = point1.position;
	triangle1.B.position = point3.position;
	triangle1.C.position = point2.position;

	triangle2.A.position = point1.position;
	triangle2.B.position = point4.position;
	triangle2.C.position = point3.position;

	triangle2.C.normal.x = 0;
	triangle2.C.normal.y = 0;
	triangle2.C.normal.z = 1;
	triangle1.A.normal = triangle1.B.normal = triangle1.C.normal = triangle2.A.normal = triangle2.B.normal = triangle2.C.normal;


	triangle1.A.alphaCo.x = 0; triangle1.A.alphaCo.y = 0;
	triangle1.B.alphaCo.x = 1; triangle1.B.alphaCo.y = 1;
	triangle1.C.alphaCo.x = 0; triangle1.C.alphaCo.y = 1;

	triangle2.A.alphaCo.x = 0; triangle2.A.alphaCo.y = 0;
	triangle2.B.alphaCo.x = 1; triangle2.B.alphaCo.y = 0;
	triangle2.C.alphaCo.x = 1; triangle2.C.alphaCo.y = 1;

	triangle1.A.textureCo = point1.textureCo;
	triangle1.B.textureCo = point3.textureCo;
	triangle1.C.textureCo = point2.textureCo;

	triangle2.A.textureCo = point1.textureCo;
	triangle2.B.textureCo = point4.textureCo;
	triangle2.C.textureCo = point3.textureCo;

	textureFront = 0;
	textureBack = 0;
	alphaRotation = 0;
}

TerrainTile::~TerrainTile()
{
}

bool TerrainTile::Intersection(RayVector ray, D3DXVECTOR3 & intersectionVertex)
{
	return intersectRayTriangle(ray, triangle1, intersectionVertex) | intersectRayTriangle(ray, triangle2, intersectionVertex);
}

std::vector<TerrainPoint>& TerrainTile::GetPoints()
{
	points.push_back(triangle1.A);
	points.push_back(triangle1.B);
	points.push_back(triangle1.C);

	points.push_back(triangle2.A);
	points.push_back(triangle2.B);
	points.push_back(triangle2.C);

	return points;
}

void TerrainTile::ClearPoints()
{
	points.clear();
}

void TerrainTile::SetTexFront(size_t textureIndex)
{
	textureFront = textureIndex;
}

void TerrainTile::SetTexBack(size_t textureIndex)
{
	textureBack = textureIndex;
}

void TerrainTile::SetTexAlpha(size_t texIndex)
{
	textureAlpha = texIndex;
}

void TerrainTile::SetAlphaRotaion(int arot)
{
	alphaRotation = arot;
}

size_t TerrainTile::GetTexFront()
{
	return textureFront;
}

size_t TerrainTile::GetTexBack()
{
	return textureBack;
}

size_t TerrainTile::GetTexAlpha()
{
	return textureAlpha;
}

int TerrainTile::GetAlphaRotation()
{
	return alphaRotation;
}


void TerrainTile::RotateAlpha(int rotateNum)
{
	alphaRotation = rotateNum;
}

bool TerrainTile::intersectRayTriangle(RayVector ray, Triangle triangle, D3DXVECTOR3 &intersectionVertex)
{
	// ������� ������� ������ ������������
	D3DXVECTOR3 u = triangle.B.position - triangle.A.position;
	D3DXVECTOR3 v = triangle.C.position - triangle.A.position;

	// ������� ������� � ������������
	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);

	if (n.x == 0 && n.y == 0 && n.z == 0) {
		return 0;                       // �������� ��������� ������������ (���� ����� �� ����� ������, ���� ��� � ����� �����)
	}

	// ������ ����������� ����
	D3DXVECTOR3 dir = ray.end - ray.begin;
	// ����� �� ����� �� ������������ �� ������ ����
	D3DXVECTOR3 w0 = ray.begin - triangle.A.position;

	float a = -D3DXVec3Dot(&n, &w0);
	float b = D3DXVec3Dot(&n, &dir);

	if (fabs(b) < 0.0001) {       // ��� ����������� ��������� ������������
		if (a == 0) { return 0; }   // ��� ����� �� ��������� ������������
		else { return 0; }   // ��� �� �� ��������� ������������

	}
	// ������ ����� ����������� ���� � �������������
	float r = a / b;
	if (r < 0.0) {
		return 0;                    // ��� ���� ���� ������������
	}
	// ��� �������� ��������� ����� (r > 1.0) => ��� �����������

	intersectionVertex = ray.begin + dir * r;           // ����� ����������� ���� � ���������
														// ����� �� ����� � ������������
	float    uu, uv, vv, wu, wv, D;
	uu = D3DXVec3Dot(&u, &u);
	uv = D3DXVec3Dot(&u, &v);
	vv = D3DXVec3Dot(&v, &v);
	D3DXVECTOR3 w = intersectionVertex - triangle.A.position;
	wu = D3DXVec3Dot(&w, &u);
	wv = D3DXVec3Dot(&w, &v);
	D = uv * uv - uu * vv;

	// ������ � �������� ��������������� ����������
	float s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0) {
		return 0;                   // ����� ��� ������������
	}
	float t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0) {
		return 0;                   // ����� ��� ������������
	}

	return 1;
}