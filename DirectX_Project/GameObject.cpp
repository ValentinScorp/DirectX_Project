#include <math.h>

#include "GameObject.h"

GameObject::GameObject()
{
	animationFrame = 0;
	animationSpeed = 0.1;
	command = 0;

	mesh = nullptr;
	animatedMesh = nullptr;
	rigidBody = nullptr;
}

GameObject::~GameObject()
{
	if (command != 0) {
		delete command;
	}	
	if (animations != nullptr) {
		delete animations;
	}
	if (animatedMesh != nullptr) {
		delete animatedMesh;
	}
	if (mesh != nullptr) {
		delete mesh;
	}
	if (rigidBody != nullptr) {
		delete rigidBody;
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

RigidBody * GameObject::GetRigidBody()
{
	return rigidBody;
}

Mesh* GameObject::GetMesh() {	
	return mesh;
}

Mesh * GameObject::GetAnimatedMesh()
{
	return animatedMesh;
}

D3DXVECTOR3 GameObject::GetPosition()
{
	if (rigidBody != nullptr) {
		return rigidBody->GetPosition();
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void GameObject::SetPosition(D3DXVECTOR3 pos)
{
	if (rigidBody != nullptr) {
		rigidBody->SetPosition(pos);
	}	
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

	animationSpeed += 1;
	if (animationSpeed >= 1) {
		animationSpeed = 0;
		animationFrame++;
	}
	if (animationFrame >= 31) {
		animationFrame = 0;
	}

	animations->AnimateMesh(animatedMesh, mesh, "Walk", animationFrame);
}

void GameObject::animate()
{



	/*
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
	for (int i = 0; i < a->GetKeyframeNum(); i++) {
		Bone b;
		b.position = a->GetKeyframe(animationFrame).positions[i];
		b.rotation = a->GetKeyframe(animationFrame).rotations[i];
		bones.push_back(b);
	}

	for (int i = 0; i < vertPositionsInit.size(); i++) {
		D3DXVECTOR3 v((float*)&vertPositionsInit[i]);
		std::vector<Weight> *weights = &vertexWeights[i];		
		
		D3DXVECTOR3 finalVecPositin(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 vertPos(v);
		D3DXMATRIX bonemat;
		D3DXMATRIX deformbonemat;
		D3DXMatrixIdentity(&deformbonemat);
		
		for (int j = 0; j < weights->size(); j++) {			
			Bone* b = (*weights)[j].bone;
			float weight = (*weights)[j].weight;
			if (b != nullptr) {				
				D3DXMatrixInverse(&bonemat, 0, &b->GetLocalToWorldMatrix());
				deformbonemat = bones[b->index].GetLocalToWorldMatrix();
				
				vertPos = v;				
				D3DXVec3TransformCoord(&vertPos, &vertPos, &bonemat);					
				D3DXVec3TransformCoord(&vertPos, &vertPos, &deformbonemat);
				vertPos *= weight;

				finalVecPositin += vertPos;								
			}
		}			
		
		vertexes[i].position.x = finalVecPositin.x;
		vertexes[i].position.y = finalVecPositin.y;
		vertexes[i].position.z = finalVecPositin.z;
	}*/
}

void GameObject::AddComponent(IObjectComponent *oc)
{
	if (oc == nullptr) {
		return;
	}
	std::string cname = oc->GetName();
	if (cname == "mesh") {
		mesh = dynamic_cast<Mesh*>(oc);
		animatedMesh = new Mesh();
		for (auto v : mesh->GetVertexesRef()) {
			animatedMesh->addVertex(v);
		}
		animatedMesh->SetTexture(mesh->GetTexture());
	}
	if (cname == "rigid_body") {
		rigidBody = dynamic_cast<RigidBody*>(oc);
	}
	if (cname == "animations") {
		animations = dynamic_cast<Animations*>(oc);
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
