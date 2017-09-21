#include "GameObject.h"

GameObject::GameObject()
{
	animationFrame = 0;
	animationSpeed = 0.1;
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

