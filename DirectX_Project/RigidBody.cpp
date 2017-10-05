#include "RigidBody.h"

RigidBody::RigidBody()
	: IObjectComponent("rigid_body"),
	position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

D3DXVECTOR3 RigidBody::GetPosition()
{
	return position;
}
