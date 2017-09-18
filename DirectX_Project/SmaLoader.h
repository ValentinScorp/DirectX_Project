#pragma once
#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <vector>

#include "Vector3D.h"

struct SmaHeader {
	char			id[10];                                    // "SMA"
	unsigned short	version;                                    //	1
};

SmaHeader smaHeader;
char objectName[32];
// ================= display data =======================
unsigned short numPositions;
Vector3Df *position;

unsigned short numNormals;
Vector3Df *normals;

unsigned short numTexcoord;
Vector2Df *textcoords;

struct TextureGroup {
	char fileName[64];
	unsigned short triangleIndices;
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

class SmaLoader
{
public:
	SmaLoader();

	void load(std::string file);

	~SmaLoader();
};

