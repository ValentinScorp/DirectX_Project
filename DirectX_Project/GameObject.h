#pragma once

#include "Vector3D.h"
#include "Bone.h"
#include "Skeleton.h"
#include "Animation.h"
#include "MessageManager.h"
#include "GameObject.h"
#include "Mesh.h"

#include <vector>

class GameObject;

struct VertexData {
	Vector3Df position;
	Vector3Df normal;
	Vector2Df uv;
};

class UnitCommand {
public:	
	~UnitCommand(){}

	virtual bool IsComplete() = 0;
	virtual void Update() = 0;
};

class MoveUnitCommand : public UnitCommand {
private:

	GameObject *gameobject;
	D3DXVECTOR3 destination;
	bool complete;
		
	float speed;
public:
	MoveUnitCommand(GameObject *go, D3DXVECTOR3 dest) 
		:gameobject(go), destination(dest), complete(0), speed(0.1) {}
	
	~MoveUnitCommand(){}

	void Update();

	bool IsComplete() {
		return complete;
	}
};

class GameObject
{
public:
	std::string name;

	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	Mesh *mesh;
	
	std::vector<Vector3Df> vertPositionsInit;
	std::vector<VertexData> vertexes;	
	std::vector<int> indexes;
	std::wstring texture = L"default.png";
	unsigned int textureId = 0;

	Skeleton skeleton;
	std::vector<Animation*> animations;
	std::vector<std::vector<Weight>> vertexWeights;

	int animationFrame;
	float animationSpeed;

	UnitCommand *command;

public:
	GameObject();
	~GameObject();

	void AddVertex(VertexData vd);
	void AddIndex(int i);

	int* GetIndexes() {
		return &indexes[0];
	}

	VertexData* GetVertexes() {
		return &vertexes[0];
	}

	unsigned int GetIndexNum() {
		return indexes.size();
	}
	unsigned int GetVertexNum() {
		return vertexes.size();
	}

	void AddMesh(Mesh *m);
	Mesh* GetMesh();

	D3DXVECTOR3 GetPosition();
	void SetPosition(D3DXVECTOR3 pos);

	void SetCommand(UnitCommand *com);
	void Update(float dt);

	void animate();
};
