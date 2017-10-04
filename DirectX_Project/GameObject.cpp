#include <math.h>

#include "GameObject.h"

GameObject::GameObject()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;

	animationFrame = 0;
	animationSpeed = 0.1;
	command = 0;

	mesh = nullptr;
}

GameObject::~GameObject()
{
	if (command != 0) {
		delete command;
	}

	for (int i = 0; i < animations.size(); i++) {
		if (animations[i] != nullptr) {
			delete animations[i];
		}
	}

	if (mesh != nullptr) {
		delete mesh;
	}

}

void GameObject::AddVertex(VertexData vd)
{
	vertexes.push_back(vd);
}

void GameObject::AddIndex(int i)
{
	indexes.push_back(i);
}

void GameObject::AddMesh(Mesh * m)
{
	if (m != 0) {
		mesh = m;
	}
}

Mesh * GameObject::GetMesh()
{
	return mesh;
}

D3DXVECTOR3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

void GameObject::SetCommand(UnitCommand * com)
{
	if (command != 0) {
		delete command;
		command = 0;
	}
	command = com;
}

void GameObject::Update(float dt)
{
	if (command != 0) {
		if (command->IsComplete()) {
			delete command;
			command = 0;
		}
		else {
			command->Update();
		}		
	}
}

void GameObject::animate()
{
	animationSpeed += 0.4;
	if (animationSpeed >= 1) {
		animationSpeed = 0;
		animationFrame++;
	}
	if (animationFrame >= 31) {
		animationFrame = 0;
	}

	Animation *a = animations[2];

	std::vector<Bone> bones;
	for (int i = 0; i < a->keyframes[0].positions.size(); i++) {
		Bone b;
		b.position = a->keyframes[animationFrame].positions[i];
		b.rotation = a->keyframes[animationFrame].rotations[i];
		bones.push_back(b);
	}

	for (int i = 0; i < vertPositionsInit.size(); i++) {
		D3DXVECTOR3 v((float*)&vertPositionsInit[i]);
		std::vector<Weight> *weights = &vertexWeights[i];		
		
		D3DXVECTOR3 finalVecPositin(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 vertPos(v);
		D3DXMATRIX bonemat;
		D3DXMATRIX bonematsum;
		D3DXMATRIX deformbonemat;
		D3DXMatrixIdentity(&deformbonemat);
		D3DXMatrixIdentity(&bonematsum);
		for (int j = 0; j < weights->size(); j++) {			
			Bone* b = (*weights)[j].bone;
			float weight = (*weights)[j].weight;
			if (b != nullptr) {				
				D3DXMatrixInverse(&bonemat, 0, &b->GetLocalToWorldMatrix());
				deformbonemat = bones[b->index].GetLocalToWorldMatrix();
				
				vertPos = v;				
				D3DXVec3TransformCoord(&vertPos, &vertPos, &bonemat);	
				//D3DXVec3TransformCoord(&vertPos, &vertPos, &b->GetLocalToWorldMatrix());
				D3DXVec3TransformCoord(&vertPos, &vertPos, &deformbonemat);
				vertPos *= weight;

				finalVecPositin += vertPos;								
			}
		}			
		
		vertexes[i].position.x = finalVecPositin.x;
		vertexes[i].position.y = finalVecPositin.y;
		vertexes[i].position.z = finalVecPositin.z;
	}
}

void MoveUnitCommand::Update()
{
	if (complete == 1) {
		return;
	}

	D3DXVECTOR3 delta = gameobject->GetPosition();
	delta = destination - delta;

	float lengthToTarget = D3DXVec3Length(&delta);	
	
	float xsign = 1;
	float ysign = 1;
	if (delta.x < 0)
		xsign = -1;
	if (delta.y < 0)
		ysign = -1;
	if (delta.x != 0) {
		float tang = abs(delta.y / delta.x);
		float rotation = atan(tang);
		delta.x = cos(rotation) * xsign;
		delta.y = sin(rotation) * ysign;
	}

	if (delta.x == 0) {
		delta.x = 1;
		delta.y = 0;
	}
	if (delta.y == 0) {
		delta.x = 0;
		delta.y = 1;
	}	
	if (lengthToTarget > (D3DXVec3Length(&delta) * speed)) {
		gameobject->SetPosition(gameobject->GetPosition() + delta * speed);
	}
	else
	{
		complete = 1;
	}
}
