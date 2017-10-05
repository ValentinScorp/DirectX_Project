#pragma once

#include <d3dx9.h>

#include "IObjectComponent.h"

class RigidBody : public IObjectComponent
{
public:
	RigidBody();
	~RigidBody();

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition();

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
};

