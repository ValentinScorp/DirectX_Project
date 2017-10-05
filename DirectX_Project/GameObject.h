#pragma once

#include "Animations.h"
#include "MessageManager.h"
#include "GameObject.h"
#include "Mesh.h"
#include "RigidBody.h"

#include <vector>

class GameObject;

struct VertexData {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
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
	
	Mesh *mesh;
	Mesh *animatedMesh;
	RigidBody *rigidBody;
	Animations *animations;

	std::vector<D3DXVECTOR3> vertPositionsInit;
	std::vector<VertexData> vertexes;	
	std::vector<int> indexes;
	
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
	Mesh* GetAnimatedMesh();
	RigidBody* GetRigidBody();

	D3DXVECTOR3 GetPosition();
	void SetPosition(D3DXVECTOR3 pos);

	void SetCommand(UnitCommand *com);
	void Update(float dt);

	void animate();

	void AddComponent(IObjectComponent *oc);
};
