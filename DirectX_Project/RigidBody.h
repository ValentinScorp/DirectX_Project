#pragma once

#include <d3dx9.h>

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition();
	D3DXMATRIX GetTransformationMatrix();
	void SetOrientationVector(D3DXVECTOR3 ovec);

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	D3DXMATRIX orientation;
};

