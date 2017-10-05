#include "Animations.h"

D3DXMATRIX Animations::Bone::GetLocalToWorldMatrix()
{
	D3DXMATRIX FinalMat;
	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
	D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
						 // pMatWorld.

						 // Using the left-to-right order of matrix concatenation,
						 // apply the translation to the object's world position
						 // before applying the rotations.
	D3DXMatrixIdentity(&FinalMat);
	D3DXMatrixTranslation(&FinalMat, position.x, position.y, position.z);
	D3DXMatrixIdentity(&MatRot);

	// Now, apply the orientation variables to the world matrix

	// Produce and combine the rotation matrices.
	D3DXMatrixRotationX(&MatTemp, rotation.x);         // Pitch
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationY(&MatTemp, rotation.y);           // Yaw
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationZ(&MatTemp, rotation.z);          // Roll
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);

	// Apply the rotation matrices to complete the world matrix.
	D3DXMatrixMultiply(&FinalMat, &MatRot, &FinalMat);

	return FinalMat;
}

Animations::Animations()
	: IObjectComponent("animation")
{
}

Animations::~Animations()
{
	weights.clear();

	for (auto a : animations) 
		if (a != nullptr) 
			delete a;

	for (auto b : bones) 
		if (b != nullptr)
			delete b;	
}

void Animations::AddBone(D3DXVECTOR3 p, D3DXVECTOR3 r)
{
	auto b = new Bone(p, r);
	bones.push_back(b);
}

Animations::Bone * Animations::GetBone(size_t id)
{
	if (id < bones.size())
		return bones[id];
	return nullptr;
}

void Animations::AddWeights(std::vector<Weight> w)
{
	weights.push_back(w);
}

void Animations::AddAnimation(Animation * anim)
{
	animations.push_back(anim);
}




