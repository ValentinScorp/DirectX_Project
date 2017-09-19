#pragma once

#include <string>
#include <vector>

#include "Vector3D.h"

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

class Animation
{
public:
	std::string name;
	std::vector <Keyframe> keyframes;
	
	Animation();
	~Animation();
};

