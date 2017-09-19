#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i] != nullptr) {
			delete animations[i];
		}
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

void GameObject::animate()
{
	Animation *a = animations[2];

	std::vector<Bone> bones;
	for (int i = 0; i < a->keyframes[1].positions.size(); i++) {
		Bone b;
		b.position = a->keyframes[1].positions[i];
		b.rotation = a->keyframes[1].rotations[i];
		bones.push_back(b);
	}

	for (int i = 0; i < vertPositionsInit.size(); i++) {
		D3DXVECTOR3 v((float*)&vertPositionsInit[i]);
		std::vector<Weight> *weights = &vertexWeights[i];
		//D3DXVECTOR3 finalVecPositin(v.x, v.y, v.z);
		D3DXVECTOR3 finalVecPositin(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < weights->size(); j++) {			
			Bone* b = (*weights)[j].bone;
			float weight = (*weights)[j].weight;
			if (b != nullptr) {
				D3DXVECTOR3 vertPos(v);

				D3DXMATRIX bonemat = b->GetLocalToWorldMatrix();
				//float d1 = D3DXMatrixDeterminant(&bonemat);
				D3DXMatrixInverse(&bonemat, 0, &bonemat);
				D3DXVec3TransformCoord(&vertPos, &vertPos, &bonemat);
				
				D3DXMATRIX deformbonemat = bones[b->index].GetLocalToWorldMatrix();
				//D3DXMatrixInverse(&deformbonemat, 0, &deformbonemat);
				D3DXVec3TransformCoord(&vertPos, &vertPos, &b->GetLocalToWorldMatrix());
				D3DXVec3Scale(&vertPos, &vertPos, weight);
							
				finalVecPositin += vertPos;
			}
		}
		
		vertexes[i].position.x = finalVecPositin.x;
		vertexes[i].position.y = finalVecPositin.y;
		vertexes[i].position.z = finalVecPositin.z;
	}
}

