#include "Bone.h"


Bone::Bone()
{
	parent = nullptr;
}


Bone::~Bone()
{
}

D3DXMATRIX Bone::GetLocalToWorldMatrix()
{
	D3DXMATRIX FinalMat;
	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
	D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
						 // pMatWorld.

						 // Using the left-to-right order of matrix concatenation,
						 // apply the translation to the object's world position
						 // before applying the rotations.
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
