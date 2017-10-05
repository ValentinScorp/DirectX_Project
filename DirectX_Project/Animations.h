#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>

#include "Vector3D.h"
#include "IObjectComponent.h"

class Keyframe
{
public:
	unsigned int index;

	std::vector<Vector3Df> rotations;
	std::vector<Vector3Df> positions;

	Keyframe() {
		index = 0;
	}
	~Keyframe() {}
};

class Animations : public IObjectComponent
{
protected:

	class Bone
	{
	public:
		Bone(D3DXVECTOR3 p, D3DXVECTOR3 r)
			:position(p), rotation(r) {}
		~Bone() {}

		D3DXMATRIX GetLocalToWorldMatrix();

	private:
		D3DXVECTOR3 position;
		D3DXVECTOR3 rotation;
	};

	class Weight
	{
	public:
		Weight(Bone *b, float w) : bone(b), weight(w) {}
		~Weight() {}

		Bone *bone;
		float weight;
	};

	class Animation {
	public:
		std::string name;
		std::vector <Keyframe> keyframes;
	};
private:
	std::vector<Bone*>bones;
	std::vector<std::vector<Weight>> weights;

	std::vector<Animation*> animations;
	
public:
	Animations();
	~Animations();

	void AddBone(D3DXVECTOR3 p, D3DXVECTOR3 r);
	Bone* GetBone(size_t id);
	void AddWeights(std::vector<Weight> w);

	void AddAnimation(Animation *anim);
	
};

