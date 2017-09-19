#pragma once

#include "Vector3D.h"

struct SmaHeader {
	char			id[8];                                    // "SMA"
	unsigned short	version;                                    //	1
};

SmaHeader smaHeader;
char objName[32];
// ================= display data =======================
unsigned short numPositions;
Vector3Df *position;

unsigned short numNormals;
Vector3Df *normals;

unsigned short numTexcoord;
Vector2Df *textcoords;

struct TextureGroup {
	char fileName[64];
	unsigned short numTriangelIndexes;
	unsigned short triangleIndexes;
};

unsigned short numTextures;
TextureGroup *texGroups;

// ================= vertex weights =======================
struct Weight {
	unsigned short boneIndex;
	float weight;
};

struct VertexWeights {
	unsigned short numWeights;
	Weight *weights;
};

unsigned short numVertexWeights;
VertexWeights *vertexWeights;

// ================= Skeleton =======================
struct Bone {
	short parentIndex;		//-1 no parent
	Vector3Df rotation;
	Vector3Df position;
};

unsigned short numBones;
Bone *bones;

// ================= Animation =======================
struct Keyframe {
	unsigned short keyframeIndex;
	Vector3Df *rotations;
	Vector3Df *positions;
};
struct Animation {
	char name[64];
	unsigned short numKeyframes;
	Keyframe *keyFrames;
};

unsigned short numAnimations;
Animation *animation;
