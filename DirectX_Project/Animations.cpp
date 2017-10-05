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
	: IObjectComponent("animations")
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

size_t Animations::GetBonesNum()
{
	return bones.size();
}

void Animations::AddVertexWeights(std::vector<Weight> w)
{
	weights.push_back(w);
}

void Animations::AddAnimation(Animation * anim)
{
	animations.push_back(anim);
}

void Animations::AnimateMesh(Mesh *m_out, Mesh *m_in, std::string aname, int frame)
{
	Animation* anim = getAnimation(aname);
	if (anim == nullptr) {
		return;
	}

	std::vector<Bone> bones;

	Keyframe *kf = anim->GetKeyframe(frame);
	
	std::vector<DxVertex>& in_v = m_in->GetVertexesRef();
	std::vector<DxVertex>& out_v = m_out->GetVertexesRef();

	for (int i = 0; i < in_v.size(); i++) {			
		D3DXVECTOR3 finalVecPositin(0.0f, 0.0f, 0.0f);				

		for (int j = 0; j < weights[i].size(); j++) {
			Bone* init_bone = weights[i][j].bone;			
			float weight = weights[i][j].weight;

			if (init_bone != nullptr) {
				Bone deform_bone(*(kf->GetPosition(init_bone)), *(kf->GetRotation(init_bone)));
				D3DXMATRIX bonemat;
				D3DXMatrixInverse(&bonemat, 0, &init_bone->GetLocalToWorldMatrix());
				D3DXMATRIX deformbonemat = deform_bone.GetLocalToWorldMatrix();

				D3DXVECTOR3 vertPos(in_v[i].position);
				D3DXVec3TransformCoord(&vertPos, &vertPos, &bonemat);
				D3DXVec3TransformCoord(&vertPos, &vertPos, &deformbonemat);
				vertPos *= weight;

				finalVecPositin += vertPos;
			}
		}

		out_v[i].position.x = finalVecPositin.x;
		out_v[i].position.y = finalVecPositin.y;
		out_v[i].position.z = finalVecPositin.z;
	}
}

Animations::Animation * Animations::getAnimation(std::string aname)
{
	for (auto a : animations) {
		if (a->GetName() == aname) {
			return a;
		}
	}
	return nullptr;
}




