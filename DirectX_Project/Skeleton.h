#pragma once

#include <vector>

#include "Bone.h"

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	void addBone(Bone *bone);

	std::vector<Bone*> bones;
};

