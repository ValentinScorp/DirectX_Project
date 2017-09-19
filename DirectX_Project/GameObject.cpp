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
				
				bonemat = b->GetLocalToWorldMatrix();				
				D3DXMatrixInverse(&bonemat, 0, &bonemat);

				bonematsum += bonemat;
				D3DXVec3TransformCoord(&vertPos, &v, &bonemat);

				deformbonemat += bones[b->index].GetLocalToWorldMatrix() * weight;		
				
				D3DXVec3TransformCoord(&vertPos, &vertPos, &deformbonemat);
				//D3DXVec3Scale(&vertPos, &vertPos, weight);
								
				finalVecPositin += vertPos;
				//break;
			}
		}
		D3DXVec3TransformCoord(&vertPos, &v, &bonematsum);
		D3DXVec3TransformCoord(&finalVecPositin, &vertPos, &deformbonemat);
		//finalVecPositin = vertPos;
		
		vertexes[i].position.x = finalVecPositin.x;
		vertexes[i].position.y = finalVecPositin.y;
		vertexes[i].position.z = finalVecPositin.z;
	}
}

