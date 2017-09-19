#include "Skeleton.h"


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i] != nullptr) {
			delete bones[i];
		}
	}
}

void Skeleton::addBone(Bone * bone)
{
	bone->index = bones.size();
	bones.push_back(bone);
}
